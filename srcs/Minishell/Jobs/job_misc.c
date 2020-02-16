/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_misc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 11:18:18 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 11:34:17 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			get_min_pos(t_job_list *job_list)
{
	t_job	*current;
	int		max;

	max = 0;
	current = (job_list && job_list->node_count) ? job_list->head : NULL;
	while (current)
	{
		if (current->pos > max)
			max = current->pos;
		current = current->next;
	}
	return (max);
}

char		current_to_char(t_job_current current)
{
	if (current == CURRENT_PREV)
		return ('-');
	else if (current == CURRENT_ACTIVE)
		return ('+');
	else
		return (' ');
}

void		print_pipes(t_process *process)
{
	while (process->next)
	{
		process = process->next;
		ft_printf("%10d %-22s | ", process->pid, " ");
		ft_print_node(process->node);
		ft_printf("\n");
	}
}

int			jobs_usage(void)
{
	ft_printf_fd(2, "jobs [-l|-p] [job_id...]\n");
	return (1);
}

int			ft_jobs_not_found(char *args)
{
	ft_printf_fd(2, "42sh: jobs: %s: no such job\n", args);
	return (1);
}
