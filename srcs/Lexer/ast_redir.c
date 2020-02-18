/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 15:58:18 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 15:58:51 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** = = = = = = = = = = = REDIRECTION PART = = = = = = = = = = = = = =
*/

t_redirection			*redirection_init(t_redirection *list)
{
	list->fd1 = -1;
	list->fd2 = -1;
	list->kind = 0;
	list->next = NULL;
	return (list);
}

t_redirection			*new_redir(int fd1, int fd2, t_token_kind kind)
{
	t_redirection *list;

	list = (t_redirection *)xmalloc(sizeof(t_redirection));
	list->fd1 = fd1;
	list->fd2 = fd2;
	list->kind = kind;
	list->word = NULL;
	list->next = NULL;
	return (list);
}

t_redirection			*reverse_redirection(t_redirection *list)
{
	t_redirection *prev;
	t_redirection *current;
	t_redirection *next;

	current = list;
	prev = NULL;
	next = NULL;
	while (current)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	return (prev);
}
