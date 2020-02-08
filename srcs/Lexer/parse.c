/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 16:14:29 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/08 12:18:20 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "shell.h"

int							is_valid_token(t_token_kind kind)
{
	if (kind == TOKEN_WORD || kind == TOKEN_ASSIGNMENT_WORD
		|| redirect_name(kind) || kind == '(' || kind == TOKEN_LBRACE)
		return (1);
	return (0);
}

void						unexpected_error(void)
{
	if (!ERRNO)
		syntax_error("syntax error near an unexpected token.");
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

void							alias_tracking()
{
	size_t				len;
	char				*tmp;
	char				*key;
	const char			*line;
	
	if ((key = get_alias(g_token.spec.word)))
	{
		line = g_token.line;
		len = g_token.start - g_token.line;
		tmp = ft_strsub(g_token.line,0, len);
		g_line = ft_strjoin(key, g_line);
		g_token.line = ft_strjoin(tmp, g_line);
		ft_strdel(&tmp);
		ft_strdel((char **)&line);
		g_line = g_token.line + len;
		escape_space();
	}
}

void							check_alias(t_list_simple_command *list)
{
	t_simple_command	*current;
	
	current = list->node_count ? list->head : NULL;
	while (current)
	{
		if(current->kind == TOKEN_WORD)
			return ;
		current = current->next;
	}
	alias_tracking();
}

t_list_simple_command		*parse_word_cmd(void)
{
	t_list_simple_command *list;

	if (ERRNO)
		return (NULL);
	list = malloc_list_simple_command();
	init_list_simple_command(list);
	while (is_token(TOKEN_WORD) || is_token(TOKEN_ASSIGNMENT_WORD))
	{
		is_token(TOKEN_WORD) ? check_alias(list) : 0;
		token_push(list, (char *)g_token.spec.word, g_token.kind);
		escape_space();
	}
	if (!list->node_count)
		free(list);
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
	}else
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
	t_node *node;

	node = parse_and_or();
	if (ERRNO || node == NULL)
		return (node);
	if (node && !ERRNO && (g_token.kind == ';' || g_token.kind == '&'))
		return (parse_sep_cmd(g_token.kind, node));
	return (node);
}
