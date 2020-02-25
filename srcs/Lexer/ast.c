/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 15:53:09 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/25 12:12:21 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parse.h"
#include "shell.h"

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
	init_list_simple_command(list);
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
	token->type = IS_NONE;
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
	if (left == NULL)
		return (right);
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
