/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 15:56:33 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 15:57:57 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** escape spaces
*/

void			escape_space(void)
{
	next_token();
	while (is_token(TOKEN_SPACE))
		next_token();
}

/*
** This function will make sure let you know if you've some uncomplete command
*/

int				get_new_line(void)
{
	char		*tmp;
	char		*tmp2;
	size_t		len;
	size_t		startlen;
	char		*new_line;

	len = g_line - g_token.line;
	startlen = g_token.start - g_token.line;
	new_line = ft_readline("> ");
	tmp = ft_strjoin(g_token.line, "\n");
	ft_strdel((char **)&g_token.line);
	tmp2 = new_line;
	if (new_line == NULL)
		return (1);
	new_line = ft_strjoin(tmp, new_line);
	ft_strdel(&tmp);
	g_token.line = new_line;
	g_token.start = new_line + startlen;
	g_line = new_line + len;
	g_token.current = ft_strjoin_pre_free(g_token.current, "\n", tmp2);
	ft_strdel(&tmp2);
	return (1);
}

/*
** escape characters and expansion shall be handled later on
*/

void			scan_dquotes(void)
{
	DECLARE(int, _(flag, 1));
	g_line++;
	while (*g_line)
	{
		if (*g_line == '"')
		{
			flag += 1;
			g_line++;
			break ;
		}
		if (*g_line && *g_line == '\\')
		{
			g_line++;
			if (*g_line == '"' || *g_line == '$')
				g_line++;
		}
		else if (*g_line)
			g_line++;
	}
	if (flag % 2 != 0)
	{
		get_new_line();
		scan_dquotes();
	}
}

void			scan_squotes(void)
{
	int flag;

	flag = 1;
	g_line++;
	while (*g_line)
	{
		if (*g_line == '\'')
		{
			flag += 1;
			g_line++;
			break ;
		}
		g_line++;
	}
	if (flag % 2 != 0)
	{
		get_new_line();
		scan_squotes();
	}
}

void			scan_meta_condition(char k1, t_token_kind k2)
{
	g_token.kind = *g_line++;
	if (*g_line == k1)
	{
		g_token.kind = k2;
		g_line++;
		if (is_metacharacter(*g_line))
			syntax_error("syntax error near unexpected token");
	}
}
