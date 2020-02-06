
#include "shell.h"


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
		{
			if (*g_line == '\\')
			{
				g_line++;
				if (*g_line == '\0')
				{
					get_new_line();
					break ;
				}
			}
			if (*g_line)
				g_line++;
		}
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

int				is_token_eof(void)
{
	return (g_token.kind == TOKEN_EOF);
}