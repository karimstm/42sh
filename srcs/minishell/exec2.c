# include "shell.h"
// to be removed later on
void		exec_commands(t_node *node, t_job_list *job_list, t_list *env);
//

void		setup_process(pid_t pgid, t_job_kind kind)
{
	pid_t pid;

	if (shell_is_interactive)
	{
		pid = getpid();
		pgid = (pgid == 0) ? pid : pgid;
		setpgid(pid, pgid);
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


void		launch_processes(t_process *p, pid_t pgid, t_job_kind kind, t_list *env)
{
	char **cmd;
	char *path;
	char **p_env;

	setup_process(pgid, kind);
	cmd = node_to_char(p->node->simple_command);
	path =  working_path(env, p->node->simple_command->head->name);
	p_env = env_to_tab(env);
	if (path == NULL)
		path = cmd[0];
	execve(path, cmd, p_env);
	exit(EXIT_FAILURE);
}

void		run_exec(t_node *node, t_job *j, t_list *env_list)
{
	t_process 	*p;
	pid_t		pid;
	p = (j->proc_list && j->proc_list->head) ? j->proc_list->head : NULL;

	while (p)
	{
		p->node = node;
		pid = fork();
		if (pid == 0)
		{
			/* Child process */
			if (shell_is_interactive)
			{
				p->pid = getpid();
				if(j->pgid == 0)
					j->pgid = p->pid;
				setpgid(j->pgid, j->pgid);
			}
			if (node->kind == NODE_SIMPLE_COMMAND)
				launch_processes(p, j->pgid, j->kind, env_list);
			else
			{
				shell_is_interactive = 0;
				t_job_list *jobs;
				jobs = (t_job_list *)xmalloc(sizeof(t_job_list));
				init_job_list(jobs);
				get_job_list(jobs);
				setup_process(j->pgid, j->kind);
				start_exec(node, env_list);
				shell_is_interactive = 1;
				exit(0);
			}
		}
		else if (pid < 0)
		{
			ft_printf_fd(2, "Fork faild\n");
			exit(1);
		}
		else
		{									/* Parent process */
			p->pid = pid; 					/* set the process pid */
			if (shell_is_interactive)
			{
				if(j->pgid == 0)
					j->pgid = pid;
				setpgid(pid, j->pgid);
			}
		}
		p = p->next;
	}
	if (!shell_is_interactive)
	{
		ft_printf_fd(2, "should not be here\n");
		wait_for_job(j);
	}
	else if (j->kind == J_FOREGROUND)
		put_job_in_foreground(j, 0);
	else if (j->kind == J_BACKGROUND)
	{
		ft_printf_fd(2, "NOR should not be here\n");
		put_job_in_background(j, 0);
	}
}

void		handle_semi_and(t_node *node, t_job_list *jobs, t_list *env)
{
	t_sep_op		*tmp_node;
	t_list_process	*procs;

	tmp_node = NULL;
	procs = (t_list_process *)xmalloc(sizeof(t_list_process));
	init_process_list(procs);
	if (node->sep_op_command->left)
	{
		tmp_node = node->sep_op_command;
		process_push(procs, 0, NULL, tmp_node->left);
		job_push(jobs, procs, 0);
		if (tmp_node->kind == '&')
			jobs->tail->kind = J_BACKGROUND;
		else
			jobs->tail->kind = J_FOREGROUND;
		exec_commands(tmp_node->left, jobs, env);
	}
	if (node->sep_op_command->right)
	{
		tmp_node = node->sep_op_command;
		process_push(procs, 0, NULL, tmp_node->right);
		job_push(jobs, procs, 0);
		if (tmp_node->kind == '&')
			jobs->tail->kind = J_BACKGROUND;
		else
			jobs->tail->kind = J_FOREGROUND;
		exec_commands(tmp_node->right, jobs, env);
	}
}

void		handle_and_or_command(t_node *node, t_job_list *jobs, t_list *env)
{
	int status = 0;

	if (jobs->tail == NULL || jobs->tail->kind == J_FOREGROUND)
	{
		if (node->and_or_command->left)
		{
			dumpy_process(jobs, node->and_or_command->left);
			exec_commands(node->and_or_command->left, jobs, env);
			status = jobs->tail->proc_list->tail->status;
		}
		if (node->and_or_command->right)
		{
			dumpy_process(jobs, node->and_or_command->left);
			if (node->and_or_command->kind == TOKEN_AND_IF && status == 0)
				exec_commands(node->and_or_command->right, jobs, env);
			else if (node->and_or_command->kind == TOKEN_OR_IF && status != 0)
				exec_commands(node->and_or_command->right, jobs, env);
		}
	}
	else if (jobs->tail->kind == J_BACKGROUND)
	{
		jobs->tail->kind = J_BACKGROUND;
		run_exec(node, jobs->tail, env);
	}
}

void		exec_commands(t_node *node, t_job_list *job_list, t_list *env)
{
	if (node)
	{
		if (node->kind == NODE_SEMI_AND)
			handle_semi_and(node, job_list, env);
		else if (node->kind == NODE_AND_OR)
			handle_and_or_command(node, job_list, env);
		else if (node->kind == NODE_SIMPLE_COMMAND)
			run_exec(node, job_list->tail, env);
	}
}
t_job_list			*start_exec(t_node *node, t_list *env)
{
	t_job_list *job_list;
	t_list_process	*procs;

	procs = NULL;
	if (!(job_list = get_job_list(NULL)))
	{
		ft_printf_fd(2, "should be here\n");
		job_list = (t_job_list *)xmalloc(sizeof(t_job_list));
		init_job_list(job_list);
		get_job_list(job_list);
	}
	if (node)
	{
		if (node->kind == NODE_SIMPLE_COMMAND)
		{
			procs = (t_list_process *)xmalloc(sizeof(t_list_process));
			init_process_list(procs);
			process_push(procs, 0, NULL, node);
			job_push(job_list, procs, 0);
		}
		exec_commands(node, job_list, env);
	}
	return (job_list);
}
