/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_jobs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 12:17:28 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/14 15:30:27 by amoutik          ###   ########.fr       */
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
		[SIGUSR1] = "User defined signal 1", "User defined signal 2",
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

void				print_pipes(t_process *process)
{
	while (process->next)
	{
		process = process->next;
		ft_printf("%10d %-22s | ", process->pid, " ");
		ft_print_node(process->node);
		ft_printf("\n");
	}
}

int					jobs_usage(void)
{
	ft_printf_fd(2, "jobs [-l|-p] [job_id...]\n");
	return (1);
}

int					ft_job_pgid(t_job *job)
{
	if (job->pgid != 0)
		ft_printf("%d\n", job->pgid);
	return (0);
}

int					ft_jobs_not_found(char *args)
{
	ft_printf_fd(2, "42sh: jobs: %s: no such job\n", args);
	return (1);
}

/*
** This function check if an option is within args
** 0 on failure & 1 on success
*/

int					ft_getopt(char **args, char *optstring, int *argc)
{
	int		i;
	int		flag;
	char	*tmp;

	i = 0;
	flag = -1;
	while (args[i])
	{
		tmp = args[i];
		if (*tmp && tmp[0] == '-')
		{
			tmp++;
			while (*tmp)
			{
				if (ft_strchr(optstring, *tmp))
					flag = *tmp;
				else
					return ('?');
				tmp++;
			}
		}
		else
			return (flag);
		*argc = ++i;
	}
	return (flag);
}

void				job_process(t_job *current, int flag)
{
	t_process		*p;
	t_list_process	*list;
	const char		*sig;

	sig = NULL;
	list = current->proc_list;
	p = list && list->head ? list->head : NULL;
	if (current->pgid != 0)
		ft_printf("[%d]%c", current->pos, current_to_char(current->current));
	if (p)
	{
		if (p->stopped && WIFSTOPPED(p->status))
			sig = ft_strsignal(WSTOPSIG(p->status));
		if (p->pid != 0)
		{
			ft_printf("%6.0d  %-22s", \
				flag == 'l' ? p->pid : 0, sig ? sig : "Running");
			ft_print_node(p->node);
			ft_printf("\n");
			if (flag == 'l')
				print_pipes(p);
		}
	}
}

void				ft_print_process(int flag)
{
	t_job *current;

	current = JOB_LIST && JOB_LIST->head ? JOB_LIST->head : NULL;
	while (current)
	{
		if (flag == 'p')
			ft_job_pgid(current);
		else if (flag == 'l' || flag == -1)
			job_process(current, flag);
		current = current->next;
	}
}


void				jobs_number(char **argv, int argc, int flag)
{
	int				i;
	char			*tmp;
	t_job			*job;

	i = argc;
	while (argv[i])
	{
		tmp = argv[i];
		if (*tmp == '%')
		{
			if (*(++tmp) == '%' || *tmp == '+' || *tmp == '\0')
				job = get_job(JOB_LIST, '+');
			else if (*tmp == '-')
				job = get_job(JOB_LIST, '+');
			else
				job = get_job(JOB_LIST, ft_atoi(tmp));
		}
		else
			job = get_job(JOB_LIST, ft_atoi(tmp));
		if (job == NULL)
			ft_jobs_not_found(argv[i]);
		else
			flag == 'l' ? job_process(job, flag) : ft_job_pgid(job);
		i++;
	}
}

int					ft_job(char **args)
{
	int flag;
	int	argc;

	argc = 0;
	if ((flag = ft_getopt(args, "lp", &argc)) == '?')
		return (jobs_usage());
	if (args == NULL || args[argc] == NULL)
		ft_print_process(flag);
	else
		jobs_number(args, argc, flag);
	return (0);
}
