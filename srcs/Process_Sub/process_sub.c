/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_sub.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:16:23 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/22 16:25:21 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		handle_sub_proc(t_string *string, char *name, int status)
{
	char		*proc;
	int			fd;
	char		*start;

	proc = scan_pro(name + 1);
	name += 2;
	char *path = setup_fifo();
	pid_t child = setup_proc(proc, path, status == '<' ? O_WRONLY : O_RDONLY);
	fd = open(path,  status == '<' ? O_RDONLY : O_WRONLY);
	ft_strdel(&proc);
	proc = dev_path(fd);
	fifo_push(FIFO_LIST, path, fd, child);
	start = proc;
	while (*proc)
		push(string, *proc++);
	ft_strdel(&start);
}

/*
** <(ls -la)
** [word]<(ls -la)[word]
** the word can be quoted
** therefore quoting need to be handled
*/

char		*process_sub_parser(char *name)
{
	t_string	string;
	int			status;

	string.string = NULL;
	new_string(&string);
	while (*name)
	{
		if ((*name == '<' && *(name + 1) == '(' && (status = '<'))
			|| (*name == '>' && *(name + 1) == '(' && (status = '>')))
		{
			handle_sub_proc(&string, name, status);
			break ;
		}
		else
			push(&string, *name);
		if (*name)
			name++;
	}
	return (string.string);
}




void		setup_sub_proce(t_list_simple_command *list)
{
	t_simple_command	*current;
	char				*name;
	current = list && list->head ? list->head : NULL;

	while (current)
	{
		name = current->name;
		current->name = process_sub_parser(current->name);
		ft_strdel(&name);
		current = current->next;
	}
}
