/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 16:14:29 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/17 11:32:04 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_node						*parse_sep_cmd(t_token_kind kind, t_node *left)
{
	t_sep_op		*sep_cmd;
	t_node			*node;

	if (ERRNO || left == NULL)
		return (left);
	sep_cmd = NULL;
	node = NULL;
	escape_space();
	sep_cmd = sep_commands(kind, left, parse_commands());
	node = command_node(NODE_SEMI_AND);
	node->spec.sep_op_command = sep_cmd;
	return (node);
}

t_node						*parse_pipe(t_token_kind kind, t_node *left)
{
	t_node		*second_node;
	t_node		*right;
	t_and_or	*pipe;

	second_node = left;
	if (ERRNO || second_node == NULL)
		return (second_node);
	while (g_token.kind == '|')
	{
		escape_space();
		if (g_token.kind == TOKEN_EOF && !error_num())
			eol_continuation();
		if ((right = init_parse_initial()) == NULL)
		{
			unexpected_error();
			break ;
		}
		pipe = and_or_commands(kind, second_node, right);
		second_node = command_node(NODE_PIPE);
		second_node->spec.and_or_command = pipe;
	}
	return (second_node);
}

t_node						*parse_pipe_and(void)
{
	t_node *node;

	node = init_parse_initial();
	if (ERRNO || node == NULL)
		return (node);
	if (g_token.kind == '|')
		return (parse_pipe(g_token.kind, node));
	return (node);
}

t_node						*parse_and_or(void)
{
	t_node			*second_node;
	t_and_or		*and_or_cmd;
	t_token_kind	kind;

	second_node = parse_pipe_and();
	if (ERRNO || second_node == NULL)
		return (second_node);
	while (g_token.kind == TOKEN_AND_IF || g_token.kind == TOKEN_OR_IF)
	{
		kind = g_token.kind;
		escape_space();
		if (g_token.kind == TOKEN_EOF)
			eol_continuation();
		if (!is_valid_token(g_token.kind))
			unexpected_error();
		and_or_cmd = and_or_commands(kind, second_node, parse_pipe_and());
		second_node = command_node(NODE_AND_OR);
		second_node->spec.and_or_command = and_or_cmd;
	}
	return (second_node);
}

t_node						*parse_commands(void)
{
	t_node			*node;
	t_token_kind	kind;

	node = parse_and_or();
	if (ERRNO || node == NULL)
		return (node);
	kind = g_token.kind;
	if (node && !ERRNO && (kind == ';' || kind == '&' || kind == TOKEN_EOF || kind == '\n'))
	{
		kind = (kind != '&') ? ';' : '&';
		return (parse_sep_cmd(kind, node));
	}
	return (node);
}
