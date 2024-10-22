/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:22:19 by amoutik           #+#    #+#             */
/*   Updated: 2020/03/02 19:12:55 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void					write_to_herdoc_fd(int fd2)
{
	char	*buf;
	char	*name;

	name = quote_stripping(g_token.spec.word);
	ft_strdel(&g_token.spec.word);
	while (1)
	{
		buf = ft_readline("> ");
		if (ft_strequ(buf, name) || ft_strequ("\4", buf) || ERRNO == EOTE)
		{
			ft_strdel(&name);
			ft_strdel(&buf);
			return ;
		}
		if ((buf = expand(buf, NULL)))
		{
			ft_printf_fd(fd2, "%s\n", buf);
			ft_strdel(&buf);
		}
	}
}

t_redirection			*here_doc(t_token_kind kind)
{
	int				fd1;
	int				fd2;
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
		write_to_herdoc_fd(fd2);
		close(fd2);
		fd2 = -1;
		list = new_redir(fd1, fd2, kind);
		list->herdoc = pathname;
	}
	else
		syntax_error("42sh: syntax error near unexpected token");
	return (list);
}
