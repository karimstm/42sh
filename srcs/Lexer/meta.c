#include "shell.h"

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
			syntax_error("syntax error near unexpected token.");
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