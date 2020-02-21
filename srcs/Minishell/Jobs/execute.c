/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 14:09:46 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/21 15:50:13 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <shell.h>

void					job_forwarding(t_job_list *job_list, t_job *job)
{
	if (job->kind == J_NON_INTERACTIVE)
		job_waiting(job_list, job);
	else if (job->kind == J_FOREGROUND)
		foreground_job(job_list, job, 0);
	else if (job->kind == J_BACKGROUND)
		background_job(job, 0);
}

void					check_pipe_and_dup(t_process *process,
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

void					sub_shell(t_process *process,
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

void					xfork(t_process *process, int pip[2],
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
		close_fifos(FIFO_LIST);
	}
}

void					execute_simple_command(t_job_list *job_list,
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
		if (cmd_type(process) == IS_BUILTIN
			&& job->proc_list->node_count == 1 && job->kind == J_FOREGROUND)
		{
			if (setup_redirection(process, 0))
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
