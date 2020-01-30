/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 16:14:29 by amoutik           #+#    #+#             */
/*   Updated: 2020/01/30 13:55:16 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lex.h"
#include "parse.h"
#include "libft.h"

int							is_valid_token(t_token_kind kind)
{
	if (kind == TOKEN_WORD || kind == TOKEN_ASSIGNMENT_WORD
		|| redirect_name(kind))
		return (1);
	return (0);
}

void						unexpected_error(void)
{
	if (!*error_num())
		syntax_error("syntax error near unexpected token");
}

void						eol_continuation()
{
	while (g_token.kind == TOKEN_EOF)
	{
		get_new_line();
		escape_space();
	}
}

const char					*token_name(t_token_kind kind)
{
	const char *token_kind_names[] = {
		[TOKEN_AND_IF] = "&&",
		[TOKEN_OR_IF] = "||",
		[TOKEN_LBRACE] = "{",
		[TOKEN_RBRACE] = "}",
		[TOKEN_DSEMI] = ";;",
		['|'] = "|"
	};

	return (token_kind_names[kind]);
}

const char					*redirect_name(t_token_kind kind)
{
	const char *redirection_name[] = {
		['<'] = "<",
		['>'] = ">",
		[TOKEN_DLESS] = "<<",
		[TOKEN_HERESTRING] = "<<<",
		[TOKEN_DLESSDASH] = "<<-",
		[TOKEN_LESSAND] = "<&",
		[TOKEN_LESSGREAT] = "<>",
		[TOKEN_DGREAT] = ">>",
		[TOKEN_DGREATAND] = ">>&",
		[TOKEN_GREATAND] = ">&",
		[TOKEN_CLOBBER] = ">|",
	};

	return (redirection_name[kind]);
}

int							grouping_kind(int kind)
{
	const int grouping_kind[] = {
		[TOKEN_LBRACE] = GROUP_BRACE_COMMAND,
		['('] = GROUP_PARAN_COMMAND,
	};

	return (grouping_kind[kind]);
}

t_list_simple_command		*parse_word_cmd(void)
{
	t_list_simple_command *list;

	if (*error_num())
		return (NULL);
	list = NULL;
	if (g_token.kind == TOKEN_WORD || g_token.kind == TOKEN_ASSIGNMENT_WORD)
	{
		list = malloc_list_simple_command();
		init_list_simple_command(list);
		while (is_token(TOKEN_WORD) || is_token(TOKEN_ASSIGNMENT_WORD))
		{
			token_push(list, (char *)g_token.spec.word, g_token.kind);
			escape_space();
		}
	}
	return (list);
}

t_redirection				*output_redir(void)
{
	t_redirection *tmp;

	tmp = NULL;
	if (g_token.kind == '>' || g_token.kind == TOKEN_CLOBBER)
		tmp = output_redirection(g_token.kind);
	else if (g_token.kind == TOKEN_DGREAT)
		tmp = output_redirection(TOKEN_DGREAT);
	else if (g_token.kind == TOKEN_GREATAND)
		tmp = output_aggregate(g_token.kind);
	else if (g_token.kind == TOKEN_DGREATAND)
		tmp = output_aggregate(g_token.kind);
	return (tmp);
}

t_redirection				*input_redir(void)
{
	t_redirection *tmp;

	tmp = NULL;
	if (g_token.kind == '<')
		tmp = input_redirection(g_token.kind);
	else if (g_token.kind == TOKEN_LESSAND)
		tmp = input_aggregate(g_token.kind);
	else if (g_token.kind == TOKEN_LESSGREAT)
		tmp = lenss_great(g_token.kind);
	else if (g_token.kind == TOKEN_DLESS || g_token.kind == TOKEN_DLESSDASH)
		tmp = here_doc(TOKEN_DLESS);
	else if (g_token.kind == TOKEN_HERESTRING)
		tmp = here_string();
	return (tmp);
}

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

t_list_simple_command		*parse_list_cmd(void)
{
	t_list_simple_command *list;

	list = NULL;
	if (g_token.kind == TOKEN_WORD)
		list = parse_word_cmd();
	return (list);
}

void						node_is_word_or_redirection(
								t_list_simple_command *list,
								t_redirection *tmp, t_redirection *redir)
{
	if (g_token.kind == TOKEN_WORD)
		list = merge_list(list, parse_word_cmd());
	else
	{
		if ((tmp = parse_redirection()) == NULL)
			return (unexpected_error());
		tmp->next = redir;
		redir = tmp;
	}
}

void						compound_command(t_node **node,
								t_redirection *redir)
{
	t_token_kind kind;

	kind = g_token.kind;
	escape_space();
	*node = parse_commands();
	if (*node == NULL || *error_num())
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
		node_is_word_or_redirection(list, tmp, redir);
	if (redir != NULL || (list && list->head))
		simple_node(&node, list, redir);
	else if (redir == NULL &&
					(g_token.kind == '(' || g_token.kind == TOKEN_LBRACE))
		compound_command(&node, redir);
	return (node);
}

t_node						*parse_sep_cmd(t_token_kind kind, t_node *left)
{
	t_sep_op		*sep_cmd;
	t_node			*node;

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

	if (*error_num())
		return (NULL);
	node = init_parse_initial();
	if (g_token.kind == '|')
		return (parse_pipe(g_token.kind, node));
	return (node);
}


t_node						*parse_and_or(void)
{
	t_node			*node;
	t_node			*second_node;
	t_and_or		*and_or_cmd;
	t_token_kind	kind;

	if (*error_num())
		return (NULL);
	node = parse_pipe_and();
	second_node = node;
	while (g_token.kind == TOKEN_AND_IF || g_token.kind == TOKEN_OR_IF)
	{
		kind = g_token.kind;
		escape_space();
		if (g_token.kind == TOKEN_EOF)
			eol_continuation();
		if (!(is_token(TOKEN_WORD) || is_token(TOKEN_ASSIGNMENT_WORD)))
			unexpected_error();
		and_or_cmd = and_or_commands(kind, second_node, parse_pipe_and());
		second_node = command_node(NODE_AND_OR);
		second_node->spec.and_or_command = and_or_cmd;
	}
	return (second_node);
}

t_node						*parse_commands(void)
{
	t_node *node;

	if (*error_num())
		return (NULL);
	node = parse_and_or();
	if (node && !*error_num() && (g_token.kind == ';' || g_token.kind == '&'))
		return (parse_sep_cmd(g_token.kind, node));
	return (node);
}
