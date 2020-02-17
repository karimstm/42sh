/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:25:28 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/17 11:39:44 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void			scan_for_digit(void)
{
	while (ft_isdigit(*g_line))
		g_line++;
	if (*g_line == '>' || *g_line == '<')
	{
		g_line = g_token.start;
		scan_int();
		scan_meta();
	}
	else
		scan_string();
}

void			set_token_as_space(void)
{
	g_token.kind = TOKEN_SPACE;
	g_line++;
}

void			next_token(void)
{
	g_token.start = g_line;
	if (*g_line)
	{
		if (*g_line == '\n')
			g_token.kind = *g_line++;
		else if (isspace(*g_line))
			set_token_as_space();
		else if (ft_isdigit(*g_line))
			scan_for_digit();
		else if (is_metacharacter(*g_line))
		{
			g_token.spec.int_val = -1;
			scan_meta();
		}
		else if (*g_line == '{' || *g_line == '}')
			scan_curly();
		else
		{
			g_token.start = g_line;
			scan_string();
		}
	}
	else
		g_token.kind = TOKEN_EOF;
	g_token.end = g_line;
}

int				is_token(t_token_kind kind)
{
	return (g_token.kind == kind);
}

int				expect_token(t_token_kind kind)
{
	if (is_token(kind))
	{
		g_token.kind = TOKEN_EOF;
		next_token();
		return (1);
	}
	else
	{
		syntax_error("syntax error near unexpected token");
		return (0);
	}
}
