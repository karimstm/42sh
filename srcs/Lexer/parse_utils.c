/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 17:30:23 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 17:36:03 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int							is_valid_token(t_token_kind kind)
{
	if (kind == TOKEN_WORD || kind == TOKEN_ASSIGNMENT_WORD
		|| redirect_name(kind) || kind == '(' || kind == TOKEN_LBRACE)
		return (1);
	return (0);
}

void						eol_continuation(void)
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
		[TOKEN_ANDGREAT] = "&>",
		[TOKEN_AND_DEGREATE] = "&>>",
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
