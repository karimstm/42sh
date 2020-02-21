/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:46:09 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/21 15:38:05 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	else if (*g_line == '(')
		init_scan_process();
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
	else if (*g_line == '(')
		init_scan_process();
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

void			and_meta(char k1, t_token_kind k2, t_token_kind k3)
{
	g_token.kind = *g_line++;
	if (*g_line == k1 || *g_line == '>')
	{
		g_token.kind = *g_line == '&' ? k2 : k3;
		g_line++;
		if (is_metacharacter(*g_line))
		{
			if (g_token.kind == TOKEN_ANDGREAT && *g_line == '>')
			{
				g_token.kind = TOKEN_AND_DEGREATE;
				g_line++;
			}
			else
				syntax_error("syntax error near unexpected token");
		}
	}
}

void			scan_meta(void)
{
	if (*g_line == '&')
		and_meta('&', TOKEN_AND_IF, TOKEN_ANDGREAT);
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
