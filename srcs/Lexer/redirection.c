/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 16:50:32 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 16:47:03 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lex.h"
#include "libft.h"

char				is_n_number(char *str, int len)
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

t_redirection		*output_redirection(t_token_kind kind)
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

void				aggregate_number(int *fd2,
						int *fd1, t_redirection **list, t_token_kind kind)
{
	*fd2 = ft_atoi(g_token.spec.word);
	*list = new_redir(*fd1, *fd2, kind);
	if (ft_strchr(g_token.spec.word, '-'))
	{
		ft_strdel(&g_token.spec.word);
		(*list)->word = ft_strdup("-");
	}
	else
		ft_strdel(&g_token.spec.word);
}

void				aggregate_word(int *fd2, int *fd1, t_redirection **list,
						t_token_kind kind)
{
	if (*fd1 < 0)
		syntax_error("42sh: %s: ambiguous redirect", g_token.spec.word);
	else
	{
		*fd2 = -1;
		(*list) = new_redir(*fd1, *fd2, kind);
		(*list)->word = g_token.spec.word;
	}
}

t_redirection		*output_aggregate(t_token_kind kind, int skip_number)
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
		if (!skip_number && is_n_number(g_token.spec.word, len))
			aggregate_number(&fd2, &fd1, &list, kind);
		else
			aggregate_word(&fd2, &fd1, &list, kind);
	}
	else
		syntax_error("42sh: syntax error near unexpected token");
	return (list);
}
