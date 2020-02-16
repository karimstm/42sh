/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 11:24:19 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 11:24:20 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		is_job_stopped(t_job *j)
{
	t_process *p;

	p = j->proc_list->head;
	while (p)
	{
		if (!p->completed && !p->stopped)
			return (0);
		p = p->next;
	}
	return (1);
}

int		is_job_completed(t_job *j)
{
	t_process *p;

	p = j->proc_list->head;
	while (p)
	{
		if (!p->completed)
			return (0);
		p = p->next;
	}
	return (1);
}
