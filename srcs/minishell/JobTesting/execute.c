#include <shell.h>


t_job_list		*get_job_list(t_job_list *jobs)
{
	static t_job_list	*list = NULL;
	if (jobs != NULL)
		list = jobs;
	return (list);
}

void		initial_process(pid_t pgid, t_job_kind kind)
{
	if (kind != J_NON_INTERACTIVE)
	{
		if (kind == J_FOREGROUND)
			tcsetpgrp(shell_terminal, pgid);
		signal (SIGINT, SIG_DFL);
		signal (SIGQUIT, SIG_DFL);
		signal (SIGTSTP, SIG_DFL);
		signal (SIGTTIN, SIG_DFL);
		signal (SIGTTOU, SIG_DFL);
		signal (SIGCHLD, SIG_DFL);
	}
}

void		setup_redirection(t_process *p, int tmp[3])
{
	if (tmp[0] != STDIN_FILENO)
	{
		dup2 (tmp[0], STDIN_FILENO);
		close (tmp[0]);
	}
	if (tmp[1] != STDOUT_FILENO)
	{
		dup2 (tmp[1], STDOUT_FILENO);
		close (tmp[1]);
	}
	if (tmp[2] != STDERR_FILENO)
	{
		dup2 (tmp[2], STDERR_FILENO);
		close (tmp[2]);
	}
	if (p->node->redir)
		execute_redirection(reverse_redirection(p->node->redir));
}

void		execute_process(t_job *job, t_process *process, t_blt_line *blt_line, int infile, int outfile, int errfile)
{
	char		**cmd;
	char		**p_env;
	int			tmp[3];

	initial_process(job->pgid, job->kind);
	/* Set the standard input/output channels of the new process.  */
	tmp[0] = infile;
	tmp[1] = outfile;
	tmp[2] = errfile;
	setup_redirection(process, tmp);
	cmd = node_to_char(process->node->simple_command);
	if (ft_lstsearch(blt_line->blt, cmd[0], &check_builtin))
	{
		exit(run_built_in(blt_line, process));
	}
	else
		execve(cmd[0], cmd, p_env);
	exit(EXIT_FAILURE);
}


int				run_built_in(t_blt_line *blt_line, t_process *process)
{
	char		**cmds;
	t_list		*bltin;

	cmds = NULL;
	if(blt_line->blt && (*(cmds = node_to_char(process->node->simple_command)) != NULL))
	{
		bltin = ft_lstsearch(blt_line->blt, cmds[0], &check_builtin);
		((t_builtin*)bltin->content)->f(cmds + 1, &blt_line->line->env);
		ft_free_strtab(cmds);
		process->status = 0; // should be related to the return value of built_in
		process->completed = 1;
		return (process->status);
	}
	return (1);
}

int			command_type(t_process *p, t_list *blt, t_list *env)
{
	char	*path;
	char	*name;

	if (p->node->kind != NODE_SIMPLE_COMMAND)
		return (0);
	name = ( p->node && p->node->simple_command && p->node->simple_command->head)
			 ? p->node->simple_command->head->name : NULL;
	if (name == NULL)
		return (1);
	if (ft_strchr(name, '/'))
		return (PATH_COMMAND);
	else if (ft_lstsearch(blt, name, &check_builtin))
		return (BUILT_IN);
	else if ((path = working_path(env, name)) == NULL)
	{
		p->status = 125;
		return (0);
	}
	else
	{
		ft_strdel(&p->node->simple_command->head->name);
		p->node->simple_command->head->name = path;
		return (1);
	}
}

void		job_forwarding(t_job_list *job_list, t_job *job)
{
	if (job->kind == J_NON_INTERACTIVE)
		job_waiting(job_list, job);
	else if (job->kind == J_FOREGROUND)
		foreground_job(job_list, job, 0);
	else if (job->kind == J_BACKGROUND)
		background_job(job_list, job, 0);
}

void		execute_simple_command(t_job_list *job_list, t_blt_line *blt_line)
{
	t_job 		*job;
	t_process	*process;
	pid_t		child;
	int			pip[2];
	int			infile;
	int			outfile;
	int			tmp[3];

	job = job_list->tail;
	process = (job) ? job->proc_list->head : NULL;

	infile = job->stdin;
	while (process)
	{
		if (process->next)
		{
			if (pipe(pip) < 0)
			{
				ft_printf_fd(2, "Pipe failed\n");
				exit(EXIT_FAILURE);
			}
			outfile = pip[1];
		}
		else
			outfile = job->stdout;
		if (command_type(process, blt_line->blt, blt_line->line->env) == BUILT_IN &&
			job->proc_list->node_count == 1 && job->kind == J_FOREGROUND)
		{
			run_built_in(blt_line, process);
			return ;
		}
		else
		{
			child = fork();
			process->pid = child;
			if (child == 0)
			{
				if (job->kind != J_NON_INTERACTIVE)
				{
					if (!job->pgid)
					{
						job->pgid = getpid();
						setpgid(job->pgid, job->pgid);
					}
					else
						setpgid(getpid(), job->pgid);
				}
				if (process->node->kind == NODE_SIMPLE_COMMAND)
					execute_process(job, process, blt_line, infile, outfile, job->stderr);
				else
				{
					t_job_list	*jobs = (t_job_list *)xmalloc(sizeof(t_job_list));
					init_job_list(jobs);
					initial_process(getpid(), job->kind);
					tmp[0] = infile;
					tmp[1] = outfile;
					tmp[2] = job->stderr;
					setup_redirection(process, tmp);
					execute_entry(jobs, process->node, blt_line, J_NON_INTERACTIVE);
					exit(0);
				}

			}
			else if (child < 0)
			{
				ft_printf_fd(2, "Fork faild\n");
				exit(EXIT_FAILURE);
			}
			else
			{
				if (job->kind != J_NON_INTERACTIVE)
				{
					if (!job->pgid)
					{
						job->pgid = child;
						setpgid(job->pgid, job->pgid);
					}
					else
						setpgid(child, job->pgid);
				}
			}
		}
		process = process->next;
		/* Pipe clean up */
		if (infile != STDIN_FILENO)
			close(infile);
		if (outfile != STDOUT_FILENO)
			close(outfile);
		infile = pip[0];
	}
	job_forwarding(job_list, job);
}

void		seperator_handling(t_job_list *job_list, t_node *node, t_blt_line *blt_line)
{
	if (node->sep_op_command->left)
	{
		if (node->sep_op_command->kind == ';')
			execute_entry(job_list, node->sep_op_command->left, blt_line, J_NON_INTERACTIVE);
		else if (node->sep_op_command->kind == '&')
			execute_entry(job_list, node->sep_op_command->left, blt_line, J_BACKGROUND);
	}
	if (node->sep_op_command->right)
	{
		if (node->sep_op_command->kind == ';')
			execute_entry(job_list, node->sep_op_command->right, blt_line, J_NON_INTERACTIVE);
		else if (node->sep_op_command->kind == '&')
			execute_entry(job_list, node->sep_op_command->right, blt_line, J_BACKGROUND);
	}
}

void		and_or_foreground(t_job_list *job_list, t_node *node, t_blt_line *blt_line, t_job_kind kind)
{
	int status;

	status = 0;
	if (node->and_or_command->left)
	{
		execute_entry(job_list, node->and_or_command->left, blt_line, kind);
		status = job_list->tail->proc_list->tail->status;
	}
	if (node->and_or_command->right)
	{

		if ((status == 0 && node->and_or_command->kind == TOKEN_AND_IF)
			|| (status && node->and_or_command->kind == TOKEN_OR_IF))
			execute_entry(job_list, node->and_or_command->right, blt_line, kind);
	}
}

void		and_or_background(t_job_list *job_list, t_node *node, t_blt_line *blt_line, t_job_kind kind)
{
	dummy_process(job_list, node, kind);
	execute_simple_command(job_list, blt_line);
}

void		and_or_handling(t_job_list *job_list, t_node *node, t_blt_line *blt_line, t_job_kind kind)
{
	if (kind == J_FOREGROUND || kind == J_NON_INTERACTIVE)
		and_or_foreground(job_list, node, blt_line, kind);
	else if (kind == J_BACKGROUND)
		and_or_background(job_list, node, blt_line, kind);
}

void		simple_command(t_job_list *job_list, t_node *node, t_blt_line *blt_line, t_job_kind kind)
{
	dummy_process(job_list, node, kind);
	execute_simple_command(job_list, blt_line);
}

void		consume_pipe(t_list_process *pro_list, t_node *node)
{
	if (node)
	{
		if (node->kind == NODE_PIPE)
		{
			if (node->and_or_command->left)
				consume_pipe(pro_list, node->and_or_command->left);
			if (node->and_or_command->right)
				consume_pipe(pro_list, node->and_or_command->right);
		}
		else
			process_push(pro_list, 0, NULL, node);
	}
}

void		pipe_entry(t_job_list *job_list, t_node *node, t_blt_line *blt_line, t_job_kind kind)
{
	t_list_process *pro_list;

	pro_list = (t_list_process *)xmalloc(sizeof(t_list_process));
	init_process_list(pro_list);
	consume_pipe(pro_list, node);
	job_push(job_list, pro_list, 0);
	job_list->tail->kind = kind;
	execute_simple_command(job_list, blt_line);
}

void		execute_entry(t_job_list *job_list, t_node *node, t_blt_line *blt_line, t_job_kind kind)
{
	if (node)
	{
		if (node->kind == NODE_SEMI_AND)
			seperator_handling(job_list, node, blt_line);
		else if (node->kind == NODE_AND_OR)
			and_or_handling(job_list, node, blt_line, kind);
		else if (node->kind == NODE_PIPE)
			pipe_entry(job_list, node, blt_line, kind);
		else if (node->kind == NODE_SIMPLE_COMMAND)
			simple_command(job_list, node, blt_line, kind);
	}
}

void		execute(t_job_list *job_list, t_node *node, t_line *line, t_list *blt)
{
	t_job_kind	kind;
	t_blt_line	*blt_line;

	kind = J_FOREGROUND;
	blt_line = (t_blt_line *)xmalloc(sizeof(t_blt_line));
	blt_line->line = line;
	blt_line->blt = blt;
	execute_entry(job_list, node, blt_line, kind);
	free(blt_line);
}
