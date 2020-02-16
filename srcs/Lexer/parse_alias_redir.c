/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_alias_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 17:28:13 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 17:37:58 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void						alias_tracking(void)
{
	size_t				len;
	char				*tmp;
	char				*key;
	const char			*line;

	if ((key = get_alias(g_token.spec.word)))
	{
		line = g_token.line;
		len = g_token.start - g_token.line;
		tmp = ft_strsub(g_token.line, 0, len);
		g_line = ft_strjoin(key, g_line);
		g_token.line = ft_strjoin(tmp, g_line);
		ft_strdel(&tmp);
		ft_strdel((char **)&line);
		line = g_line;
		g_line = g_token.line + len;
		ft_strdel(&tmp);
		ft_strdel((char **)&line);
		ft_strdel(&g_token.spec.word);
		escape_space();
	}
}

void						check_alias(t_list_simple_command *list)
{
	t_simple_command	*current;

	current = list->node_count ? list->head : NULL;
	while (current)
	{
		if (current->kind == TOKEN_WORD)
			return ;
		current = current->next;
	}
	alias_tracking();
}

t_list_simple_command		*parse_word_cmd(void)
{
	t_list_simple_command *list;

	if (ERRNO)
		return (NULL);
	list = malloc_list_simple_command();
	init_list_simple_command(list);
	while (is_token(TOKEN_WORD) || is_token(TOKEN_ASSIGNMENT_WORD))
	{
		is_token(TOKEN_WORD) ? check_alias(list) : 0;
		token_push(list, (char *)g_token.spec.word, g_token.kind);
		escape_space();
	}
	if (!list->node_count)
	{
		free(list);
		list = NULL;
	}
	return (list);
}

t_redirection				*output_redir(void)
{
	t_redirection *tmp;

	tmp = NULL;
	if (g_token.kind == '>' || g_token.kind == TOKEN_CLOBBER)
		tmp = output_redirection(g_token.kind);
	else if (g_token.kind == TOKEN_DGREAT)
		tmp = output_redirection(TOKEN_DGREAT);
	else if (g_token.kind == TOKEN_ANDGREAT)
		tmp = output_aggregate(g_token.kind, 1);
	else if (g_token.kind == TOKEN_AND_DEGREATE)
		tmp = output_aggregate(g_token.kind, 1);
	else if (g_token.kind == TOKEN_GREATAND)
		tmp = output_aggregate(g_token.kind, 0);
	else if (g_token.kind == TOKEN_DGREATAND)
		tmp = output_aggregate(g_token.kind, 0);
	return (tmp);
}

t_redirection				*input_redir(void)
{
	t_redirection *tmp;

	tmp = NULL;
	if (g_token.kind == '<')
		tmp = input_redirection(g_token.kind);
	else if (g_token.kind == TOKEN_LESSAND)
		tmp = input_aggregate(g_token.kind);
	else if (g_token.kind == TOKEN_LESSGREAT)
		tmp = lenss_great(g_token.kind);
	else if (g_token.kind == TOKEN_DLESS || g_token.kind == TOKEN_DLESSDASH)
		tmp = here_doc(TOKEN_DLESS);
	else if (g_token.kind == TOKEN_HERESTRING)
		tmp = here_string();
	return (tmp);
}
