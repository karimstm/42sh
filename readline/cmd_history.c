/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_history.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 08:55:45 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/13 12:08:41 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"

static	t_cmd_history *g_history_list = NULL;

t_cmd_history	*get_cmd_history_head(void)
{
	return (g_history_list);
}

void			add_to_history(const char *str, int len, char from_readline)
{
	t_cmd_history	*tmp;

	if (!str)
		return ;
	if ((tmp = ft_memalloc(sizeof(t_cmd_history))))
	{
		if (g_history_list)
			g_history_list->prev = tmp;
		tmp->index = g_history_list ? g_history_list->index + 1 : 1;
		tmp->next = g_history_list;
		tmp->line = ft_strdup(str);
		tmp->tmp_line = ft_strdup(str);
		tmp->len = len;
		tmp->tmp_len = len;
		if (from_readline != 1) {
			if (!history_begining)
				history_begining = tmp;
			if (!new_history_begining)
				new_history_begining = tmp;
		}
		g_history_list = tmp;
	}
}

void			free_history(void)
{
	t_cmd_history	*next;

	while (g_history_list)
	{
		next = g_history_list->next;
		if (*g_history_list->line)
		{
			ft_bzero(g_history_list->line, g_history_list->len);
			free(g_history_list->line);
			ft_strdel(&g_history_list->tmp_line);
		}
		free(g_history_list);
		g_history_list = next;
	}
}

void			clean_hsitory(void)
{
	t_cmd_history	*tmp;

	if (g_history_list)
	{
		tmp = g_history_list->next;
		free(g_history_list->line);
		free(g_history_list->tmp_line);
		free(g_history_list);
		g_history_list = tmp;
		if (tmp != NULL)
			tmp->prev = NULL;
	}
}

void			set_cur_history(t_readline *readline, t_cmd_history *cur)
{
	char	*new_line;

	if (!cur)
		return ;
	if (ft_strequ(readline->cmd->line, readline->cmd->tmp_line))
		ft_strdel(&readline->cmd->tmp_line);
	readline->cmd = cur;
	if (!cur->tmp_line && (new_line = ft_strdup(cur->line)))
	{
		cur->tmp_line = new_line;
		cur->tmp_len = cur->len;
	}
	readline->line_props.details = get_line_details(readline);
	if (cur->tmp_line && readline->line_props.details)
	{
		readline->line_index = cur->tmp_len;
		set_cursor_from_index(readline);
		rewrite_line(readline);
	}
}
