/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_operation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 22:57:02 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/10 23:56:00 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"

void		rewrite_line(t_readline *readline)
{
	const t_point	cursor = readline->o_cursor;

	tputs(tgoto(tgetstr("cm", 0), cursor.x, cursor.y), 0, put_char);
	update_o_cursor(readline);
	tputs(tgetstr("cd", NULL), 0, put_char);
	ft_putstr(readline->cmd->tmp_line);
	cur_goto(readline, readline->cursor);
}

static void	put_new_line(t_readline *readline, char *new_line,
														int is_line, int len)
{
	free(readline->cmd->tmp_line);
	readline->cmd->tmp_line = new_line;
	readline->cmd->tmp_len += len;
	readline->line_props.details[readline->line_props.index] += len;
	if (is_line)
		readline->line_props.details = get_line_details(readline);
	if (readline->line_props.details)
	{
		set_cursor_from_index(readline);
		rewrite_line(readline);
	}
}

void		insert_in_line(t_readline *readline, char *str)
{
	char		*new_line;
	const int	is_line = !!ft_strchr(str, '\n');
	const int	len = ft_strlen(str);

	if (str)
	{
		new_line = ft_strnew(readline->cmd->tmp_len + len);
		ft_strncpy(new_line, readline->cmd->tmp_line, readline->line_index);
		ft_strcat(new_line, str);
		ft_strcat(new_line, readline->cmd->tmp_line + readline->line_index);
		if (new_line)
		{
			readline->line_index += len;
			put_new_line(readline, new_line, is_line, len);
		}
	}
}

void		remove_from_line(t_readline *readline, int start, int end)
{
	char		*new_line;
	char *const	line = readline->cmd->tmp_line;
	int			is_line;
	const int	len = end - start + 1;

	if (start >= 0)
	{
		is_line = !!ft_strnstr(&line[start], "\n", len);
		line[start] = 0;
		new_line = ft_strjoin(line, line + end + 1);
		readline->line_index = start;
		if (new_line)
			put_new_line(readline, new_line, is_line, -len);
	}
}

int			*get_line_details(t_readline *readline)
{
	int		*ret;
	char	*tmp;
	int		i;
	char	*line;

	i = 0;
	line = readline->cmd->tmp_line;
	ft_memdel((void **)&readline->line_props.details);
	readline->line_props.linecount = 1 + ft_str_occurence(line, '\n');
	if ((ret = ft_memalloc(sizeof(int) * readline->line_props.linecount)))
		while (i < readline->line_props.linecount)
		{
			tmp = (char *)ft_skip_unitl_char(line, "\n", NULL);
			ret[i++] = (tmp - line) + (int)(*tmp == '\n');
			line = tmp + 1;
		}
	return (ret);
}
