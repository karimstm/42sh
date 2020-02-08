#include <shell.h>

t_job_list		*get_job_list(t_job_list *jobs)
{
	static t_job_list	*list = NULL;

	if (jobs != NULL)
		list = jobs;
	return (list);
}

void			initial_process(pid_t pgid, t_job_kind kind)
{
	if (kind != J_NON_INTERACTIVE)
	{
		if (kind == J_FOREGROUND)
			ft_tcsetpgrp(shell_terminal, pgid);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		signal(SIGCHLD, SIG_DFL);
	}
}

int			setup_redirection(t_process *p, int doexit)
{
	if (p->node->redir)
	{
		if (execute_redirection(reverse_redirection(p->node->redir)))
		{
			if (doexit)
				exit(EXIT_FAILURE);
			return (0);
		}
	}
	return (1);
}

void			execute_process(t_job *job, t_process *process,
								t_blt_line *blt_line, int pip[2])
{
	char		**cmd;
	char		**p_env;

	cmd = NULL;
	initial_process(job->pgid, job->kind);
	(pip[0] != -1) ? close(pip[0]) : 0;
	(pip[1] != -1) ? close(pip[1]) : 0;
	setup_redirection(process, EXIT_FAILURE);
	if (SIMPLE_CMD(process->node) && SIMPLE_CMD(process->node)->node_count)
		cmd = node_to_char(SIMPLE_CMD(process->node));
	else
		exit(EXIT_SUCCESS);
	p_env = get_tab_env();
	if (ft_lstsearch(get_set_blt(NULL), cmd[0], &check_builtin))
		exit(run_built_in(blt_line, process));
	else if (cmd)
		execve(cmd[0], cmd, p_env);
	exit(EXIT_FAILURE);
}

int				run_built_in(t_blt_line *blt_line, t_process *process)
{
	char		**cmds;
	t_list		*bltin;

	cmds = NULL;
	if (blt_line->blt
		&& (*(cmds = node_to_char(process->node->spec.simple_command)) != NULL))
	{
		bltin = ft_lstsearch(blt_line->blt, cmds[0], &check_builtin);
		process->status = ((t_builtin*)bltin->content)->f(cmds + 1);
		ft_free_strtab(cmds);
		return (process->status);
	}
	return (1);
}

t_simple_command	*get_command_name(t_list_simple_command *list)
{
	t_simple_command	*current;

	current = list->node_count ? list->head : NULL;
	while (current)
	{
		if (current->kind == TOKEN_WORD)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_cmd_type				cmd_type(t_process *p, t_list *blt)
{
	char				*path;
	char				*name;
	t_simple_command	*cmd;

	cmd = NULL;
	if (p->node->kind != NODE_SIMPLE_COMMAND)
		return (0);
	if (SIMPLE_CMD(p->node))
		cmd = get_command_name(SIMPLE_CMD(p->node));
	name = cmd && cmd->name ? cmd->name : NULL;
	if (name == NULL)
		return (1);
	if (ft_strchr(name, '/'))
		return (cmd->type = IS_PATH_CMD);
	else if (ft_lstsearch(blt, name, &check_builtin))
		return (cmd->type = IS_BUILTIN);
	else if ((path = working_path(name)) == NULL)
	{
		p->status = 127;
		return (cmd->type = IS_NOTFOUND);
	}
	else
	{
		ft_strdel(&cmd->name);
		cmd->name = path;
		return (cmd->type = IS_FOUND);
	}
}

void			job_forwarding(t_job_list *job_list, t_job *job)
{
	if (job->kind == J_NON_INTERACTIVE)
		job_waiting(job_list, job);
	else if (job->kind == J_FOREGROUND)
		foreground_job(job_list, job, 0);
	else if (job->kind == J_BACKGROUND)
		background_job(job, 0);
}

void			set_fds(int tmp_stds[3])
{
	tmp_stds[0] = dup3(0);
	tmp_stds[1] = dup3(1);
	tmp_stds[2] = dup3(2);
}

void			check_pipe_and_dup(t_process *process,
						int *infile, int tmp[3], int pip[2])
{
	dup2(*infile, STDIN_FILENO);
	close(*infile);
	if (process->next)
	{
		if (pipe(pip) < 0)
		{
			ft_printf_fd(2, "Pipe failed\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			*infile = pip[0];
			dup2(pip[1], STDOUT_FILENO);
			close(pip[1]);
		}
	}
	else
		dup2(tmp[1], STDOUT_FILENO);
}

void			setup_pgid(pid_t child, t_job *job)
{
	if (!job->pgid)
		job->pgid = child;
	setpgid(child, job->pgid);
}

void			sub_shell(t_process *process,
					t_job *job,
					t_blt_line *blt_line,
					int pip[2])
{
	t_job_list *jobs;

	jobs = (t_job_list *)xmalloc(sizeof(t_job_list));
	init_job_list(jobs);
	initial_process(getpid(), job->kind);
	setup_redirection(process, EXIT_FAILURE);
	(pip[0] != -1) ? close(pip[0]) : 0;
	(pip[1] != -1) ? close(pip[1]) : 0;
	execute_entry(jobs, process->node, blt_line, J_NON_INTERACTIVE);
	free(jobs);
	exit(0);
}

void			xfork(t_process *process, int pip[2],
					t_job *job, t_blt_line *blt_line)
{
	pid_t child;

	child = fork();
	if (child == 0)
	{
		process->pid = getpid();
		if (job->kind != J_NON_INTERACTIVE)
			setup_pgid(getpid(), job);
		if (process->node->kind == NODE_SIMPLE_COMMAND)
			execute_process(job, process, blt_line, pip);
		else
			sub_shell(process, job, blt_line, pip);
	}
	else if (child < 0)
	{
		ft_printf_fd(2, "Fork faild\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		process->pid = child;
		if (job->kind != J_NON_INTERACTIVE)
			setup_pgid(child, job);
	}
}

void			execute_simple_command(t_job_list *job_list,
								t_blt_line *blt_line)
{
	t_job		*job;
	t_process	*process;
	int			pip[2];
	int			tmp[3];
	int			infile;
	pip[0] = -1;
	pip[1] = -1;
	job = job_list->tail;
	process = (job) ? job->proc_list->head : NULL;
	set_fds(tmp);
	infile = dup3(STDIN_FILENO);
	while (process)
	{
		check_pipe_and_dup(process, &infile, tmp, pip);
		if (cmd_type(process, blt_line->blt) == IS_BUILTIN
			&& job->proc_list->node_count == 1 && job->kind == J_FOREGROUND)
		{
			if(setup_redirection(process, 0))
				run_built_in(blt_line, process);
			else
				process->status = 1;
			restore_std(tmp);
			return ;
		}
		else
			xfork(process, pip, job, blt_line);
		process = process->next;
	}
	restore_std(tmp);
	job_forwarding(job_list, job);
}

/*
** I changed J_NON_INTERACTIVE TO J_FOREGROUND
** STILL I DON'T KNOW WHY I USED NON INTERACTIVE MODE HERE
** thus I used kind = getpid() != shell_pgid ? J_NON_INTERACTIVE : J_FOREGROUND
*/
void			seperator_handling(t_job_list *job_list,
								t_node *node, t_blt_line *blt_line)
{
	t_job_kind kind;

	kind = getpid() != shell_pgid ? J_NON_INTERACTIVE : J_FOREGROUND;
	if (LEFT(node))
	{
		if (node->spec.sep_op_command->kind == ';')
			execute_entry(job_list, LEFT(node), blt_line, kind);
		else if (node->spec.sep_op_command->kind == '&')
			execute_entry(job_list, LEFT(node), blt_line, J_BACKGROUND);
	}
	if (RIGHT(node))
	{
		if (node->spec.sep_op_command->kind == ';')
			execute_entry(job_list, RIGHT(node), blt_line, kind);
		else if (node->spec.sep_op_command->kind == '&')
			execute_entry(job_list, RIGHT(node), blt_line, J_BACKGROUND);
	}
}

void			and_or_foreground(t_job_list *job_list, t_node *node,
								t_blt_line *blt_line, t_job_kind kind)
{
	int		status;

	status = 0;
	if (LEFT_A(node))
	{
		execute_entry(job_list, LEFT_A(node), blt_line, kind);
		status = job_list->tail->proc_list->tail->status;
	}
	if (RIGHT_A(node))
	{
		if ((status == 0 && node->spec.and_or_command->kind == TOKEN_AND_IF)
			|| (status && node->spec.and_or_command->kind == TOKEN_OR_IF))
			execute_entry(job_list, RIGHT_A(node), blt_line, kind);
	}
}

void			and_or_background(t_job_list *job_list, t_node *node,
								t_blt_line *blt_line, t_job_kind kind)
{
	dummy_process(job_list, node, kind);
	execute_simple_command(job_list, blt_line);
}

void			and_or_handling(t_job_list *job_list, t_node *node,
							t_blt_line *blt_line, t_job_kind kind)
{
	if (kind == J_FOREGROUND || kind == J_NON_INTERACTIVE)
		and_or_foreground(job_list, node, blt_line, kind);
	else if (kind == J_BACKGROUND)
		and_or_background(job_list, node, blt_line, kind);
}

void			simple_command(t_job_list *job_list,
							t_node *node, t_blt_line *blt_line, t_job_kind kind)
{
	dummy_process(job_list, node, kind);
	execute_simple_command(job_list, blt_line);
}

void			consume_pipe(t_list_process *pro_list, t_node *node)
{
	if (node)
	{
		if (node->kind == NODE_PIPE)
		{
			if (node->spec.and_or_command->left)
				consume_pipe(pro_list, node->spec.and_or_command->left);
			if (node->spec.and_or_command->right)
				consume_pipe(pro_list, node->spec.and_or_command->right);
		}
		else
			process_push(pro_list, 0, NULL, node);
	}
}

void			pipe_entry(t_job_list *job_list,
						t_node *node, t_blt_line *blt_line, t_job_kind kind)
{
	t_list_process		*pro_list;

	pro_list = (t_list_process *)xmalloc(sizeof(t_list_process));
	init_process_list(pro_list);
	consume_pipe(pro_list, node);
	job_push(job_list, pro_list, 0);
	job_list->tail->kind = kind;
	execute_simple_command(job_list, blt_line);
}

void			execute_entry(t_job_list *job_list, t_node *node,
							t_blt_line *blt_line, t_job_kind kind)
{
	int				tmp[3];
	char			*name;
	t_simple_command *cmd;

	if (node->redir && kind == J_FOREGROUND && node->kind != NODE_SIMPLE_COMMAND)
	{
		set_fds(tmp);
		execute_redirection(reverse_redirection(node->redir));
	}
	if (node)
	{
		if (node->kind == NODE_SEMI_AND)
			seperator_handling(job_list, node, blt_line);
		else if (node->kind == NODE_AND_OR)
			and_or_handling(job_list, node, blt_line, kind);
		else if (node->kind == NODE_PIPE)
			pipe_entry(job_list, node, blt_line, kind);
		else if (node->kind == NODE_SIMPLE_COMMAND)
		{
			cmd = get_command_name(node->spec.simple_command);
			name = cmd && cmd->name ? ft_strdup(cmd->name) : NULL;
			simple_command(job_list, node, blt_line, kind);
			if (name && cmd && cmd->type == IS_FOUND)
				ht_insert(get_hash_table(NULL), name, cmd->name);
			ft_strdel(&name);
		}
	}
	if (node->redir && kind == J_FOREGROUND && node->kind != NODE_SIMPLE_COMMAND)
		restore_std(tmp);
}

void			execute(t_job_list *job_list, t_node *node,
						t_line *line, t_list *blt)
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
