/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 12:31:02 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/25 14:50:04 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void			setup_pgid(pid_t child, t_job *job)
{
	if (!job->pgid)
		job->pgid = child;
	setpgid(child, job->pgid);
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

int				setup_redirection(t_process *p, int doexit)
{
	if (p->node->redir)
	{
		expand_redirection(p->node->redir);
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
		&& ((cmds = node_to_char(process->node->spec.simple_command)) != NULL)
		&& *cmds)
	{
		if ((bltin = ft_lstsearch(blt_line->blt, cmds[0], &check_builtin)))
			process->status = ((t_builtin*)bltin->content)->f(cmds + 1);
		ft_free_strtab(cmds);
		process->completed = 1;
		close_fifos(FIFO_LIST);
		return (process->status);
	}
	return (1);
}
