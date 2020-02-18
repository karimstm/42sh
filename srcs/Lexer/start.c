/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 18:32:53 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 17:25:35 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int				*error_num(void)
{
	static int error = 0;

	return (&error);
}

void			syntax_error(const char *fmt, ...)
{
	va_list	args;

	va_start(args, fmt);
	ft_vprintf(2, fmt, &args);
	ft_printf_fd(2, "\n");
	va_end(args);
	g_token.kind = TOKEN_EOF;
	g_line = NULL;
	ERRNO = SYNTAXE;
}

int				is_ifs(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

uint8_t			get_digit(uint8_t index)
{
	const uint8_t char_to_digit[256] = {['0'] = 0,
		['1'] = 1,
		['2'] = 2,
		['3'] = 3,
		['4'] = 4,
		['5'] = 5,
		['6'] = 6,
		['7'] = 7,
		['8'] = 8,
		['9'] = 9,
		['a'] = 10,
		['A'] = 10,
		['b'] = 11,
		['B'] = 11,
		['c'] = 12,
		['C'] = 12,
		['d'] = 13,
		['D'] = 13,
		['e'] = 14,
		['E'] = 14,
		['f'] = 15,
		['F'] = 15,
	};

	return (char_to_digit[index]);
}

int				is_metacharacter(int ch)
{
	const char metacharacter[256] = {
		['|'] = '|',
		['&'] = '&',
		[';'] = ';',
		['('] = '(',
		[')'] = ')',
		['<'] = '<',
		['>'] = '>',
	};

	return (metacharacter[ch]);
}
