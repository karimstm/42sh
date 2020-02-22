/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subs_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:47:18 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/22 15:25:20 by amoutik          ###   ########.fr       */
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

void			scan_dollar(void)
{
	DECLARE(int, _(index, 0), stack[100]);
	g_line++;
	if (*g_line == '(')
	{
		while (*g_line)
		{
			if(*g_line == '"')
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
	{
		while (*g_line && *g_line != '}')
			g_line++;
		g_line++;
	}
}

void			scan_process()
{
	DECLARE(int, _(index, 0), stack[100]);
	if (*g_line == '(')
	{
		while (*g_line)
		{
			if(*g_line == '"')
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

int				globa_scanner(void)
{
	if (*g_line == '"')
		scan_dquotes();
	else if (*g_line == '\'')
		scan_squotes();
	else if (*g_line == '$')
		scan_dollar();
	else
		return (0);
	return (1);
}

void			init_scan_process()
{
	g_token.kind = TOKEN_WORD;
	scan_process();
	if (g_line == NULL)
		return ;
	scan_string();
}

int			scan_backslash()
{
	if (*g_line == '\\')
	{
		g_line++;
		if (*g_line == '\0')
		{
			get_new_line();
				if (g_line && *g_line == '\n')
				{
					g_line++;
					return (-1);
				}
			return(1);
		}
	}
	return (0);
}

void			scan_string(void)
{
	int		status;

	g_token.kind = TOKEN_WORD;
	check_assignment();
	while (*g_line)
	{
		if ((*g_line == '<' || *g_line == '>') && *(g_line + 1) == '(')
		{
			g_line++;
			scan_process();
		}
		if (is_metacharacter(*g_line) || is_ifs(*g_line)
			|| *g_line == '}' || *g_line == '{')
			break;
		if (!globa_scanner())
		{
			if ((status = scan_backslash()) == -1)
				continue ;
			else if (status == 1)
				break ;
			if (*g_line)
				g_line++;
		}
	}
	g_token.spec.word = ft_strsub(g_token.start, 0, g_line - g_token.start);
}

void			scan_int(void)
{
	int64_t val;
	int64_t base;
	int64_t digit;

	val = 0;
	base = 10;
	g_token.spec.int_val = -1;
	while (ft_isdigit(*g_line))
	{
		digit = get_digit(*g_line);
		val = val * base + digit;
		g_line++;
	}
	g_token.spec.int_val = val;
}

/*
**	int				is_token_eof(void)
**	{
**		return (g_token.kind == TOKEN_EOF);
**	}
*/
