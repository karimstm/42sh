/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:15:46 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 16:22:36 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** ['>'] = ">",
** 		[TOKEN_DLESS] = "<<",
** 		[TOKEN_HERESTRING] = "<<<",
** 		[TOKEN_DLESSDASH] = "<<-",
** 		[TOKEN_LESSAND] = "<&",
** 		[TOKEN_LESSGREAT] = "<>",
** 		[TOKEN_DGREAT] = ">>",
** 		[TOKEN_DGREATAND] = ">>&",
** 		[TOKEN_GREATAND] = ">&",
** 		[TOKEN_CLOBBER] = ">|",
*/

t_redirection			*input_redirection(t_token_kind kind)
{
	int				fd1;
	int				fd2;
	t_redirection	*list;

	list = NULL;
	fd1 = g_token.spec.int_val == -1 ? 0 : g_token.spec.int_val;
	fd2 = -1;
	escape_space();
	if (g_token.kind == TOKEN_WORD)
	{
		list = new_redir(fd1, fd2, kind);
		list->word = g_token.spec.word;
	}
	else
		syntax_error("42sh: syntax error near unexpected token");
	return (list);
}

t_redirection			*here_string(void)
{
	int				fd1;
	int				fd2;
	t_redirection	*list;

	list = NULL;
	fd1 = g_token.spec.int_val == -1 ? 0 : g_token.spec.int_val;
	escape_space();
	fd2 = 1;
	list = new_redir(fd1, fd2, TOKEN_HERESTRING);
	list->herdoc = ft_strsub(g_token.start, 0, g_token.end - g_token.start);
	if (g_token.kind == TOKEN_WORD)
		ft_strdel(&g_token.spec.word);
	return (list);
}

t_redirection			*dashed_input(t_token_kind kind, int fd1)
{
	t_redirection	*list;

	list = NULL;
	if (g_token.spec.word && ft_strcmp(g_token.spec.word, "-") == 0)
	{
		list = new_redir(fd1, -1, kind);
		list->word = g_token.spec.word;
	}
	else
		syntax_error("42sh: %s: file number expected",\
	g_token.spec.word);
	return (list);
}

t_redirection			*input_aggregate(t_token_kind kind)
{
	int				fd1;
	int				fd2;
	int				len;
	t_redirection	*list;

	list = NULL;
	fd1 = g_token.spec.int_val == -1 ? 0 : g_token.spec.int_val;
	escape_space();
	if (g_token.kind == TOKEN_WORD)
	{
		len = ft_strlen(g_token.spec.word);
		if (g_token.spec.word[len - 1] == '-')
			len -= 1;
		if (is_n_number(g_token.spec.word, len))
			aggregate_number(&fd2, &fd1, &list, kind);
		else
			list = dashed_input(kind, fd1);
	}
	else
		syntax_error("42sh: syntax error near unexpected token");
	return (list);
}

/*
** Less and great <>
*/

t_redirection			*lenss_great(t_token_kind kind)
{
	int				fd1;
	t_redirection	*list;

	list = NULL;
	fd1 = g_token.spec.int_val == -1 ? 0 : g_token.spec.int_val;
	escape_space();
	if (g_token.kind == TOKEN_WORD)
	{
		list = new_redir(fd1, -1, kind);
		list->word = g_token.spec.word;
	}
	else
		syntax_error("42sh: syntax error near unexpected token");
	return (list);
}
