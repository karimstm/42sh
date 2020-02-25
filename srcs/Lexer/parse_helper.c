/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 17:27:34 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/25 12:50:00 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_redirection				*parse_redirection(void)
{
	t_redirection *head;
	t_redirection *tmp;

	head = NULL;
	tmp = NULL;
	while (redirect_name(g_token.kind))
	{
		tmp = output_redir();
		if (tmp == NULL)
			tmp = input_redir();
		if (tmp == NULL)
			return (head);
		tmp->next = head;
		head = tmp;
		tmp = NULL;
		escape_space();
	}
	return (head);
}

void						node_is_word_or_redirection(
								t_list_simple_command **list,
								t_redirection *tmp, t_redirection **redir)
{
	if (g_token.kind == TOKEN_WORD)
		*list = merge_list(*list, parse_word_cmd());
	else
	{
		if ((tmp = parse_redirection()) == NULL)
			return (unexpected_error());
		tmp->next = *redir;
		*redir = tmp;
	}
}

/*
** Parse compound command or as known as  grouping commands
*/

void						compound_command(t_node **node,
								t_redirection *redir)
{
	t_token_kind kind;

	kind = g_token.kind;
	escape_space();
	*node = parse_commands();
	if (*node == NULL || ERRNO)
		return (unexpected_error());
	(*node)->goup_kind = grouping_kind(kind);
	if ((kind == '(' && is_token(')'))
		|| (kind == TOKEN_LBRACE && is_token(TOKEN_RBRACE)))
	{
		escape_space();
		while (redirect_name(g_token.kind))
			redir = parse_redirection();
		(*node)->redir = redir;
	}
	else
		unexpected_error();
}

void						simple_node(t_node **node,
								t_list_simple_command *list,
								t_redirection *redir)
{
	(*node) = command_node(NODE_SIMPLE_COMMAND);
	(*node)->spec.simple_command = list;
	(*node)->redir = redir;
}

t_node						*init_parse_initial(void)
{
	t_list_simple_command		*list;
	t_redirection				*redir;
	t_redirection				*tmp;
	t_node						*node;

	node = NULL;
	tmp = NULL;
	list = parse_word_cmd();
	redir = parse_redirection();
	while (g_token.kind == TOKEN_WORD || redirect_name(g_token.kind))
		node_is_word_or_redirection(&list, tmp, &redir);
	if (redir != NULL || (list && list->head))
		simple_node(&node, list, redir);
	else if (redir == NULL &&
					(g_token.kind == '(' || g_token.kind == TOKEN_LBRACE))
		compound_command(&node, redir);
	return (node);
}
