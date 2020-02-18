/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fg.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 14:09:51 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/15 14:26:03 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			fg_usage(char c)
{
	if (c)
		ft_printf_fd(2, "42sh: fg: -%c: invalid option\n", c);
	ft_printf_fd(2, "fg: usage: fg [job_spec]\n");
	return (-1);
}

int			fg_error(char *job)
{
	ft_printf_fd(2, "42sh: fg: %s: no such job\n", job);
	return (-1);
}

int			get_job_number(char **args)
{
	char	*tmp;

	if (args == NULL || *args == NULL)
		return (0);
	DECLARE(int, _(job_number, -1));
	if (args != NULL)
		if ((tmp = args[0]))
		{
			if (*tmp == '%')
				tmp++;
			if (*tmp)
			{
				if (*tmp == '+')
					return ('+');
				else if (*tmp == '-' && tmp[1] == '0')
					return ('-');
				else if (is_number(tmp))
					return (ft_atoi(tmp));
			}
			if (args[0][0] == '-' && args[0][1])
				return (fg_usage(args[0][1]));
			else
				return (fg_error(args[0]));
		}
	return (job_number);
}

t_job		*get_job(t_job_list *jobs, int job_number)
{
	t_job *current;

	current = jobs->head;
	while (current)
	{
		if ((job_number == '+' || job_number == 0)
				&& current->current == CURRENT_ACTIVE)
			return (current);
		else if (job_number == '-' && current->current == CURRENT_PREV)
			return (current);
		else if (current->pos == job_number)
			return (current);
		else if (jobs->node_count == 1 && job_number == '-')
			return (current);
		current = current->next;
	}
	return (current);
}

int			ft_fg(char **args)
{
	t_job		*current;
	t_job_list	*list;
	int			job_number;
	int			error;

	error = 0;
	list = get_job_list(NULL);
	if ((job_number = get_job_number(args)) == -1)
		error = fg_error("current");
	else if ((current = get_job(list, job_number)))
	{
		if (current->pgid == 0)
			error = fg_error(args[0]);
		else
			continue_job(list, current, 1);
	}
	else
		error = fg_error(args[0]);
	return (error ? 1 : 0);
}
