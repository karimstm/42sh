/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 11:10:03 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/21 20:10:43 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_fifo_list		*fifo_list(t_fifo_list *list)
{
	static t_fifo_list	*new = NULL;
	if (list != NULL)
		new = list;
	return (new);
}

void		init_fifo_list(t_fifo_list *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->node_count = 0;
}

void		fifo_push(t_fifo_list *list, char *pathname, int fd, pid_t child)
{
	t_fifo *file;

	file = (t_fifo*)xmalloc(sizeof(t_fifo));
	file->pathname = pathname;
	file->fd = fd;
	file->child = child;
	file->next = NULL;
	if (list->node_count == 0)
		list->head = file;
	else
		list->tail->next = file;
	list->tail = file;
	list->node_count++;
}

void	close_fifos(t_fifo_list *list)
{
	t_fifo	*current;
	t_fifo	*next;

	current = list && list->head ? list->head : NULL;
	while (current)
	{
		next = current->next;
		if (current->fd != -1)
			close(current->fd);
		unlink(current->pathname);
		ft_strdel(&current->pathname);
		kill(current->child, SIGCONT);
		waitpid(current->child, NULL, 0);
		free(current);
		current = next;
		list->node_count--;
	}
	list->head = NULL;
	list->head = NULL;
}

char	*dev_path(int fd)
{
	const char *dirname = "/dev/fd/";
	char		*cfd;
	char		*path;

	cfd = ft_itoa(fd);
	path = ft_strjoin(dirname, cfd);
	ft_strdel(&cfd);
	return (path);
}
