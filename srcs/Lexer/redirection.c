/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 16:50:32 by amoutik           #+#    #+#             */
/*   Updated: 2020/01/24 18:35:52 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lex.h"
#include "libft.h"

static char				ft_isnumber(char *str, int len)
{
	int i;

	i = 0;
	if (str == NULL || *str == '\0' || len == 0)
		return (0);
	while (*str && i < len)
	{
		if (!ft_isdigit(*str))
			return (0);
		i++;
		str++;
	}
	return (1);
}

t_redirection			*output_redirection(t_token_kind kind)
{
	int					fd1;
	int					fd2;
	t_redirection		*list;

	list = NULL;
	fd1 = g_token.spec.int_val == -1 ? 1 : g_token.spec.int_val;
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

void					aggregate_number(int *fd2,
							int *fd1, t_redirection **list)
{
	*fd2 = ft_atoi(g_token.spec.word);
	*list = new_redir(*fd1, *fd2, TOKEN_GREATAND);
	if (ft_strchr(g_token.spec.word, '-'))
	{
		ft_strdel(&g_token.spec.word);
		(*list)->word = ft_strdup("-");
	}
	else
		ft_strdel(&g_token.spec.word);
}

void					aggregate_word(int *fd2, int *fd1, t_redirection **list)
{
	if (*fd1 < 0)
		syntax_error("42sh: %s: ambiguous redirect", g_token.spec.word);
	else
	{
		*fd2 = -1;
		(*list) = new_redir(*fd1, *fd2, TOKEN_GREATAND);
		(*list)->word = g_token.spec.word;
	}
}

t_redirection			*output_aggregate(void)
{
	int				fd1;
	int				fd2;
	int				len;
	t_redirection	*list;

	list = NULL;
	fd1 = g_token.spec.int_val == -1 ? 1 : g_token.spec.int_val;
	escape_space();
	if (g_token.kind == TOKEN_WORD)
	{
		len = ft_strlen(g_token.spec.word);
		if (g_token.spec.word[len - 1] == '-')
			len -= 1;
		if (ft_isnumber(g_token.spec.word, len))
			aggregate_number(&fd2, &fd1, &list);
		else
			aggregate_word(&fd2, &fd1, &list);
	}
	else
		syntax_error("42sh: syntax error near unexpected token");
	return (list);
}

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

t_redirection			*here_doc(t_token_kind kind)
{
	int				fd1;
	int				fd2;
	char			*buf;
	char			*pathname;
	t_redirection	*list;

	list = NULL;
	fd1 = g_token.spec.int_val == -1 ? 0 : g_token.spec.int_val;
	fd2 = -1;
	escape_space();
	if (g_token.kind == TOKEN_WORD)
	{
		pathname = ft_tmpfile();
		fd2 = open_tmp(pathname);
		while (1)
		{
			buf = readline("> ");
			if (ft_strcmp(buf, g_token.spec.word) == 0)
			{
				ft_strdel(&buf);
				break ;
			}
			ft_printf_fd(fd2, "%s\n", buf);
			ft_strdel(&buf);
		}
		close(fd2);
		fd2 = -1;
		list = new_redir(fd1, fd2, kind);
		list->herdoc = pathname;
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
	list->herdoc = ft_strndup(g_token.start, g_token.end - g_token.start);
	if (g_token.kind == TOKEN_WORD)
		ft_strdel(&g_token.spec.word);
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
		if (ft_isnumber(g_token.spec.word, len))
			aggregate_number(&fd2, &fd1, &list);
		else
		{
			if (g_token.spec.word && ft_strcmp(g_token.spec.word, "-") == 0)
			{
				list = new_redir(fd1, -1, kind);
				list->word = g_token.spec.word;
			}
			else
				syntax_error("42sh: %s: file number expected",\
					g_token.spec.word);
		}
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
