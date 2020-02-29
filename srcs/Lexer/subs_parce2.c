/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subs_parce2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 13:53:27 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/29 13:53:32 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void			scan_curly(void)
{
	if (*g_line == '{')
		g_token.kind = TOKEN_LBRACE;
	else if (*g_line == '}')
		g_token.kind = TOKEN_RBRACE;
	g_line++;
}

void			check_assignment(void)
{
	if (ft_isalpha(*g_line) || *g_line == '_')
	{
		while (ft_isalnum(*g_line) || *g_line == '_')
			g_line++;
		if (*g_line == '=')
		{
			g_token.kind = TOKEN_ASSIGNMENT_WORD;
			g_line++;
		}
	}
}

void			scan_dollar_param(void)
{
	while (*g_line && *g_line != '}')
		g_line++;
	g_line++;
}

void			scan_dollar(void)
{
	DECLARE(int, _(index, 0), stack[100]);
	g_line++;
	if (*g_line == '(')
	{
		while (*g_line)
		{
			if (*g_line == '"')
				scan_dquotes();
			else if (*g_line == '\'')
				scan_squotes();
			else if (*g_line == '\\')
				g_line++;
			if (*g_line == '(')
				stack[index++] = '(';
			else if (*g_line == ')')
				index--;
			if (index == 0)
				break ;
			if (*g_line)
				g_line++;
		}
		g_line++;
	}
	else if (*g_line == '{')
		scan_dollar_param();
}

void			scan_process(void)
{
	DECLARE(int, _(index, 0), stack[100]);
	if (*g_line == '(')
	{
		while (*g_line)
		{
			if (*g_line == '"')
				scan_dquotes();
			else if (*g_line == '\'')
				scan_squotes();
			else if (*g_line == '\\')
				g_line++;
			if (*g_line == '(')
				stack[index++] = '(';
			else if (*g_line == ')')
				index--;
			if (index == 0)
				break ;
			g_line++;
		}
		g_line++;
	}
	g_token.kind = TOKEN_WORD;
	if (index)
		unexpected_error();
}
