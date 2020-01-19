/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_jobs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 12:17:28 by amoutik           #+#    #+#             */
/*   Updated: 2020/01/19 17:16:23 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	ft_print_node(t_node *node);

const char		*ft_strsignal(int sig)
{
	static const char *signal_str[] = {
		[SIGHUP] = "Hangup",
		[SIGINT] = "Interrupt",
		[SIGQUIT] = "Quit",
		[SIGILL] = "Illegal instruction",
		[SIGTRAP] = "Trace/breakpoint trap",
		[SIGABRT] = "Aborted",
		[SIGEMT] = "Emulation trap",
		[SIGFPE] = "Arithmetic exception",
		[SIGBUS] = "Bus error",
		[SIGSEGV] = "Segmentation fault",
		[SIGSYS] = "Bad system call",
		[SIGPIPE] = "Broken pipe",
		[SIGALRM] = "Alarm clock",
		[SIGTERM] = "Terminated",
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

void	print_semi_and(t_node *node)
{
	if (node->sep_op_command->left)
		ft_print_node(node->sep_op_command->left);
	ft_printf(" %c ", node->sep_op_command->kind);
	if (node->sep_op_command->right)
		ft_print_node(node->sep_op_command->right);
}

void	print_and_or_pipe(t_node *node)
{
	if (node->and_or_command->left)
		ft_print_node(node->and_or_command->left);
	ft_printf(" %c ", node->and_or_command->kind);
	if (node->and_or_command->right)
		ft_print_node(node->and_or_command->right);
}

void	ft_print_node(t_node *node)
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
			print_list_tokens(node->simple_command);
		}
	}
}

void		ft_process(t_job *job)
{
	t_process *process;

	process = (job->proc_list && job->proc_list->head)
				? job->proc_list->head : NULL;
	while (process)
	{
		ft_printf("%s        ", ft_strsignal(WTERMSIG(process->status)));
		ft_print_node(process->node);
		process = process->next;
	}
}

void		ft_jobs(char **args, t_list **env, t_job_list *list)
{
	t_job		*current;
	int			i;

	(void)args;
	(void)env;
	current = (list && list->head) ? list->head : NULL;
	i = list->node_count;
	while (current)
	{
		ft_printf("[%d]+  ", i--);
		ft_process(current);
		current = current->next;
	}
}
