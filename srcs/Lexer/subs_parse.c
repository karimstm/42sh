/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subs_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:47:18 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/29 13:54:59 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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

void			init_scan_process(void)
{
	g_token.kind = TOKEN_WORD;
	scan_process();
	if (g_line == NULL)
		return ;
	scan_string();
}

int				scan_backslash(void)
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
			return (1);
		}
	}
	return (0);
}

void			scan_string(void)
{
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
			break ;
		if (!globa_scanner())
		{
			DECLARE(int, status);
			if ((status = scan_backslash()) == -1)
				continue ;
			else if (status == 1)
				break ;
			if (*g_line)
				g_line++;
		}
	}
	g_token.spec.word = !ERRNO ?\
		ft_strsub(g_token.start, 0, g_line - g_token.start) : ft_strdup("");
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
