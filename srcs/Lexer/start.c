/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 18:32:53 by amoutik           #+#    #+#             */
/*   Updated: 2020/01/24 18:32:54 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex.h"
#include "parse.h"
#include "libft.h"

int				*error_num(void)
{
	static int error = 0;

	return (&error);
}

void			reset_error_num(void)
{
	int *error;

	error = error_num();
	*error = 0;
}

void			syntax_error(const char *fmt, ...)
{
	va_list	args;
	int		*error;

	va_start(args, fmt);
	ft_vprintf(2, fmt, &args);
	ft_printf_fd(2, "\n");
	va_end(args);
	g_token.kind = TOKEN_EOF;
	g_line = NULL;
	error = error_num();
	*error = 1;
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
	new_line = readline("> ");
	tmp = ft_strjoin(g_token.line, "\n");
	free((char *)g_token.line);
	tmp2 = new_line;
	new_line = ft_strjoin(tmp, new_line);
	ft_strdel(&tmp2);
	g_token.line = new_line;
	g_token.start = new_line + startlen;
	g_line = new_line + len;
	return (1);
}

/*
** escape characters and expansion shall be handled later on
*/

void			scan_dquotes(void)
{
	int flag;

	flag = 1;
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
	while (*g_line && !is_ifs(*g_line))
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
			syntax_error("near unexpected token `%c'", *g_line);
	}
}

void			scan_redirection_input(void)
{
	g_token.kind = *g_line++;
	if (*g_line == '<')
	{
		g_token.kind = TOKEN_DLESS;
		g_line++;
		if (*g_line == '<' || *g_line == '-')
		{
			g_token.kind = *g_line == '<' ? TOKEN_HERESTRING : TOKEN_DLESSDASH;
			g_line++;
		}
	}
	else if (*g_line == '&' || *g_line == '>')
	{
		g_token.kind = *g_line == '&' ? TOKEN_LESSAND : TOKEN_LESSGREAT;
		g_line++;
	}
}

void			scan_redirection_output(void)
{
	g_token.kind = *g_line++;
	if (*g_line == '>')
	{
		g_token.kind = TOKEN_DGREAT;
		g_line++;
		if (*g_line == '&')
		{
			g_token.kind = TOKEN_DGREATAND;
			g_line++;
		}
	}
	else if (*g_line == '&' || *g_line == '|')
	{
		g_token.kind = *g_line == '&' ? TOKEN_GREATAND : TOKEN_CLOBBER;
		g_line++;
	}
}

void			scan_meta_semi(void)
{
	g_token.kind = *g_line++;
	if (*g_line == ';')
	{
		g_token.kind = TOKEN_DSEMI;
		g_line++;
		if (is_metacharacter(*g_line))
			syntax_error("near unexpected token `%c'", *g_line);
	}
}

void			scan_meta(void)
{
	if (*g_line == '&')
		scan_meta_condition('&', TOKEN_AND_IF);
	else if (*g_line == '|')
		scan_meta_condition('|', TOKEN_OR_IF);
	else if (*g_line == ';')
		scan_meta_semi();
	else if (*g_line == '<')
		scan_redirection_input();
	else if (*g_line == '>')
		scan_redirection_output();
	else
		g_token.kind = *g_line++;
}

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
	const char		*start;
	int				stack[100];
	int				index;

	index = 0;
	start = g_line;
	g_line++;
	if (*g_line == '(')
	{
		while (*g_line)
		{
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
	else if (*g_line == '{')
	{
		while (*g_line && *g_line != '}')
			g_line++;
		g_line++;
	}
	g_token.start = start;
}

void			scan_string(void)
{
	g_token.kind = TOKEN_WORD;
	check_assignment();
	while (*g_line && !is_metacharacter(*g_line) &&
			!is_ifs(*g_line) && *g_line != '}' && *g_line != '{')
	{
		if (*g_line == '"')
			scan_dquotes();
		else if (*g_line == '\'')
			scan_squotes();
		else if (*g_line == '$')
			scan_dollar();
		else
			g_line++;
	}
	g_token.spec.word = ft_strndup(g_token.start, g_line - g_token.start);
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

void			next_token(void)
{
	g_token.start = g_line;
	if (*g_line)
	{
		if (isspace(*g_line))
		{
			g_token.kind = TOKEN_SPACE;
			g_line++;
		}
		else if (ft_isdigit(*g_line))
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
	{
		g_token.kind = TOKEN_EOF;
	}
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
		return (0);
}

int				is_token_eof(void)
{
	return (g_token.kind == TOKEN_EOF);
}

int				match_token(t_token_kind kind)
{
	if (is_token(kind))
	{
		next_token();
		return (1);
	}
	else
		return (0);
}

int				match_token_test(t_token_kind kind)
{
	while (is_token(TOKEN_SPACE))
		next_token();
	if (is_token(kind))
	{
		next_token();
		return (1);
	}
	else
		return (0);
}

void			init_stream(const char *str)
{
	g_line = str;
	g_token.line = str;
	escape_space();
}
