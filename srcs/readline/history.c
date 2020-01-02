/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 17:03:51 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/05/08 17:47:32 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	print_history(t_line *line, char *new_line, char flag_free_line)
{
	int		i;

	go_home(line);
	tputs(tgetstr("cd", NULL), 1, ft_putchar);
	if (flag_free_line)
		free_line();
	line = init_line();
	i = -1;
	while (new_line[++i])
		print_char_inline(line, new_line[i]);
	free(new_line);
}

static void	up_history(t_line *line)
{
	char	flag_tmp_history_saved;
	char	*new_line;

	flag_tmp_history_saved = 0;
	if (line->index_history)
		line->index_history = line->index_history->previous ?
			line->index_history->previous : line->index_history;
	else if (*(line->tail_history))
	{
		line->index_history = *(line->tail_history);
		line->tmp_history = line->command;
		flag_tmp_history_saved = 1;
	}
	else
		return ;
	new_line = ft_strnew(ft_strlen(line->index_history->content));
	ft_strcpy(new_line, line->index_history->content);
	print_history(line, new_line, !flag_tmp_history_saved);
}

static void	down_history(t_line *line)
{
	char	*new_line;

	if (line->index_history)
		line->index_history = line->index_history->next;
	else
		return ;
	if (line->index_history)
	{
		new_line = ft_strnew(ft_strlen(line->index_history->content));
		ft_strcpy(new_line, line->index_history->content);
	}
	else
	{
		new_line = ft_strnew(ft_strlen(line->tmp_history));
		ft_strcpy(new_line, line->tmp_history);
		free(line->tmp_history);
		line->tmp_history = NULL;
	}
	print_history(line, new_line, 1);
}

void		handle_history(int buf, t_line *line)
{
	if (buf == UP_KEY)
		up_history(line);
	else if (buf == DOWN_KEY)
		down_history(line);
}

void		m_add_history(t_line *line)
{
	t_list	**history;
	t_list	*new_history;

	history = line->tail_history;
	new_history = ft_lstnew(line->command, ft_strlen(line->command));
	ft_lstadd_end(history, new_history);
	*history = new_history;
	line->index_history = NULL;
}
