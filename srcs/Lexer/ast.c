/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 15:53:09 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/06 12:54:47 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parse.h"
#include "shell.h"

void					*xmalloc(size_t num_bytes)
{
	void *ptr;

	ptr = malloc(num_bytes);
	if (!ptr)
	{
		ft_printf_fd(2, "xmalloc failed");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

t_node					*command_node(t_token_node kind)
{
	t_node *node;

	node = (t_node *)xmalloc(sizeof(t_node));
	node->kind = kind;
	node->redir = NULL;
	return (node);
}

t_list_simple_command	*malloc_list_simple_command(void)
{
	t_list_simple_command *list;

	list = (t_list_simple_command *)xmalloc(sizeof(t_list_simple_command));
	return (list);
}

void					init_list_simple_command(t_list_simple_command *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->node_count = 0;
}

void					token_push(t_list_simple_command *list,
									char *token_str, t_token_kind kind)
{
	t_simple_command *token;

	token = (t_simple_command *)xmalloc(sizeof(t_simple_command));
	token->name = token_str;
	token->kind = kind;
	token->next = NULL;
	if (list->node_count == 0 || list->head == NULL)
		list->head = token;
	else
		list->tail->next = token;
	list->tail = token;
	list->node_count++;
}

t_list_simple_command	*merge_list(t_list_simple_command *left,
									t_list_simple_command *right)
{
	if (right == NULL)
		return (left);
	if (left && right)
		left->node_count += right->node_count;
	if (left->head && right->head)
	{
		left->tail->next = right->head;
		left->tail = right->tail;
	}
	else if (right)
		return (right);
	return (left);
}