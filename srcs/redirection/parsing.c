/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 23:21:38 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/05/11 18:36:12 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void		parse_string(char **doc, char *line, char flag_dollar)
{
	char		*str;
	t_string	str_dollar;

	if (flag_dollar && ft_strchr(line, '$'))
	{
		str_dollar.string = NULL;
		new_string(&str_dollar);
		while (*line)
		{
			if (*line == '$')
				handle_dollar(&line, &str_dollar);
			push(&str_dollar, *line);
			line++;
		}
		str = ft_strdup(str_dollar.string);
		free_string(&str_dollar);
	}
	else
		str = ft_strdup(line);
	if (*doc != NULL)
		*doc = ft_strjoin_pre_free(*doc, "\n", str);
	else
		*doc = ft_strdup(str);
	ft_strdel(&str);
}

static char		*getdoc(t_token *token, char *flag_stop)
{
	t_line	*line;
	char	*doc;
	char	flag_dollar;

	free_line();
	line = init_line();
	line->print_msg = 0;
	doc = NULL;
	flag_dollar = 0;
	if (!(token->tok_type & SH_QUOTED) && !(ft_strchr(token->token, '\'')
			|| ft_strchr(token->token, '"')))
		flag_dollar = 1;
	while (read_line(line) == 0 && ft_strcmp(line->command, token->token) != 0
			&& line->command[line->index] != EOT_KEY && line->print_msg == 0)
	{
		parse_string(&doc, line->command, flag_dollar);
		free_line();
		init_line();
		line->print_msg = 0;
	}
	if (line->print_msg)
		*flag_stop = 1;
	doc = (doc == NULL) ? ft_strdup("") : doc;
	return (doc);
}

char			parse_heredoc(t_token_list *tokens)
{
	t_token		*ptr;
	t_token		*tmp_node;
	char		flag_stop;

	ptr = tokens->head;
	flag_stop = 0;
	while (ptr)
	{
		if ((ptr->tok_type & SH_REDIRECTION) && ft_strstr(ptr->token, "<<")
				&& ptr->next)
		{
			tmp_node = (t_token*)ft_memalloc(sizeof(t_token));
			tmp_node->tok_type = ptr->next->tok_type;
			tmp_node->token = getdoc(ptr->next, &flag_stop);
			tmp_node->next = ptr->next->next;
			ft_strdel(&(ptr->next->token));
			free(ptr->next);
			ptr->next = tmp_node;
		}
		if (flag_stop == 1)
			return (-1);
		ptr = ptr->next;
	}
	return (0);
}
