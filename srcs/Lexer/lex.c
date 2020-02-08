#include "shell.h"

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
	{
		syntax_error("syntax error near unexpected token");
		return (0);
	}
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
	g_token.current = ft_strdup(str);
	escape_space();
}
