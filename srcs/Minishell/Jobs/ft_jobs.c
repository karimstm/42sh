/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_jobs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 12:17:28 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/12 21:00:59 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

const char			*ft_strsignal(int sig)
{
	static const char *signal_str[] = {
		[SIGHUP] = "Hangup",
		[SIGQUIT] = "Quit",
		"Illegal instruction", "Trace/breakpoint trap", "Aborted",
		"Emulation trap", "Arithmetic exception",
		"Killed", "Bus error", "Segmentation fault",
		"Bad system call", "Broken pipe", "Alarm clock", "Terminated",
		[SIGUSR1] = "User defined signal 1",
		[SIGUSR2] = "User defined signal 2",
		[SIGCHLD] = "Child status changed",
		[SIGWINCH] = "Window size changed",
		[SIGURG] = "Urgent I/O condition",
		[SIGIO] = "I/O possible",
		[SIGSTOP] = "Stopped (signal)",
		[SIGTSTP] = "Stopped (user)",
		[SIGCONT] = "Continued",
		[SIGTTIN] = "Stopped (tty input)",
		[SIGTTOU] = "Stopped (tty output)",
		[SIGVTALRM] = "Virtual timer expired",
		[SIGPROF] = "Profiling timer expired",
		[SIGXCPU] = "CPU time limit exceede",
		[SIGXFSZ] = "File size limit exceeded",
		[SIGINFO] = "Information request"
	};

	return (signal_str[sig]);
}

void				print_semi_and(t_node *node)
{
	if (node->spec.sep_op_command->left)
		ft_print_node(node->spec.sep_op_command->left);
	ft_printf("%c", node->spec.sep_op_command->kind);
	if (node->spec.sep_op_command->right)
		ft_print_node(node->spec.sep_op_command->right);
}

void				print_and_or_pipe(t_node *node)
{
	if (node->spec.and_or_command->left)
		ft_print_node(node->spec.and_or_command->left);
	ft_printf("%s", token_name(node->spec.and_or_command->kind));
	if (node->spec.and_or_command->right)
		ft_print_node(node->spec.and_or_command->right);
}

void				ft_print_node(t_node *node)
{
	if (node)
	{
		if (node->kind == NODE_SEMI_AND)
			print_semi_and(node);
		else if (node->kind == NODE_AND_OR || node->kind == NODE_PIPE)
			print_and_or_pipe(node);
		else if (node->kind == NODE_SIMPLE_COMMAND)
		{
			ft_printf(" ");
			print_list_tokens(node->spec.simple_command);
		}
		print_redir(node->redir);
	}
}

char				current_to_char(t_job_current current)
{
	if (current == CURRENT_PREV)
		return ('-');
	else if (current == CURRENT_ACTIVE)
		return ('+');
	else
		return (' ');
}

void				print_pipes(t_process *process, char flag)
{
	while (process->next && flag == 'l')
	{
		process = process->next;
		ft_printf("%10d %-22s | ", process->pid, " ");
		ft_print_node(process->node);
		ft_printf("\n");
	}
}

void				ft_process(t_job *job, char flag)
{
	t_process	*process;
	char		*sig;

	sig = NULL;
	process = (job->proc_list && job->proc_list->head)
				? job->proc_list->head : NULL;
	if (job->pgid != 0)
		ft_printf("[%d]%c", job->pos, current_to_char(job->current));
	if (process)
	{
		if (process->stopped && WIFSTOPPED(process->status))
			sig = (char *)ft_strsignal(WSTOPSIG(process->status));
		if (process->pid != 0)
		{
			ft_printf("%6d  %-22s", process->pid, sig ? sig : "Running");
			ft_print_node(process->node);
			while (process->next && flag == 0)
			{
				process = process->next;
				ft_print_node(process->node);
			}
			ft_printf("\n");
			print_pipes(process, flag);
		}
	}
}

int					jobs_usage(void)
{
	ft_printf_fd(2, "jobs [-l|-p] [job_id...]\n");
	return (1);
}

int					parse_args(char **args)
{
	const char	*options = "lp";
	char		*tmp;

	DECLARE(int, _(flag, 0), _(i, 0));
	if (args != NULL)
		while (args[i])
		{
			if (args[i][0] == '-')
			{
				tmp = args[i] + 1;
				while (*tmp)
				{
					if (ft_strchr(options, *tmp) == NULL)
					{
						ft_printf_fd(2, "jobs: -%c: invalid option\n", *tmp);
						return (jobs_usage());
					}
					else
						flag = *tmp;
					tmp++;
				}
			}
			i++;
		}
	return (flag);
}

int					ft_job_pgid(t_job *job)
{
	if (job->pgid != 0)
		ft_printf("%d", job->pgid);
	return (0);
}

int					ft_jobs_not_found(char *args)
{
	ft_printf_fd(2, "42sh: jobs: %s: no such job\n", args);
	return (1);
}

int					ft_jobs(char **args)
{
	t_job		*current;
	t_job_list	*list;
	int			flag;

	list = get_job_list(NULL);
	flag = 0;
	if ((flag = parse_args(args)) == 1)
		return (1);
	current = (list && list->head) ? list->head : NULL;
	while (current)
	{
		if (flag == 'l' || flag == 0)
			ft_process(current, (char)flag);
		else if (flag == 'p')
			ft_job_pgid(current);
		current = current->next;
	}
	return (0);
}
