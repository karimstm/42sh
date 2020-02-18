/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_jobs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 12:17:28 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 11:17:22 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int					ft_job_pgid(t_job *job)
{
	if (job->pgid != 0)
		ft_printf("%d\n", job->pgid);
	return (0);
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
