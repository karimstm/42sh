/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paste.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 22:57:23 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/04/22 12:26:04 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <shell.h>

static void	add_char(t_line *line, char c)
{
	char	*tmp;

	if (c == '\0')
		return ;
	if (line->top + 2 >= line->buf_size)
		line->buf_size *= 2;
	tmp = ft_strnew(line->buf_size);
	ft_strncpy(tmp, line->command, line->index + 1);
	tmp[line->index + 1] = c;
	ft_strncpy(tmp + line->index + 2,
			line->command + line->index + 1, line->top - line->index);
	tmp[++line->top + 1] = '\0';
	free(line->command);
	line->command = tmp;
	line->index++;
}

static void	get_pasted_str(t_line *line, char **str)
{
	char	buf;
	int		i;
	char	begin;

	i = -1;
	begin = 0;
	while (read(0, &buf, 1) > 0)
	{
		if (begin == 2 && buf == 'R')
			break ;
		else if (begin == 1 && buf == 91)
			begin = 2;
		else if (begin == 0 && buf == 27)
			begin = 1;
		else if (begin == 0 && (ft_isprint(buf) ||
					buf == '\n' || buf == '\t'))
		{
			if (i + 2 >= line->buf_size)
			{
				*str = ft_realloc(*str, line->buf_size * 2, i + 1);
				line->buf_size *= 2;
			}
			str[0][++i] = buf;
		}
	}
}

static void	paste_chars(int *buf, t_line *line)
{
	char	*str;
	char	*c;
	int		i;
	int		j;

	str = (char*)ft_memalloc(sizeof(char) * line->buf_size);
	tputs("\E[6n", 1, ft_putchar);
	get_pasted_str(line, &str);
	c = (char*)buf;
	i = -1;
	while (++i < 4 && (ft_isprint(c[i]) || c[i] == '\n' || c[i] == '\t'))
	{
		if (c[i] == '\t')
		{
			j = -1;
			while (++j < 4)
				add_char(line, ' ');
		}
		else
			add_char(line, c[i]);
	}
	i = -1;
	while (str[++i])
		add_char(line, str[i]);
	free(str);
}

void		print_pasted_chars(int *buf, t_line *line)
{
	int		diff;
	int		index;
	char	*tmp;

	free_next_newlines(line);
	diff = line->top - line->index;
	index = line->index;
	paste_chars(buf, line);
	line->index = index;
	tmp = ft_strdup(line->command + line->index + 1);
	update_line(line, tmp, -1);
	free(tmp);
	while (diff-- > 0)
		go_left(line);
}

void		internal_paste(t_line *line)
{
	int		i;
	int		diff;
	int		index;
	char	*tmp;

	free_next_newlines(line);
	diff = line->top - line->index;
	index = line->index;
	i = -1;
	while (line->copy[++i])
		add_char(line, line->copy[i]);
	line->index = index;
	tmp = ft_strdup(line->command + line->index + 1);
	update_line(line, tmp, -1);
	free(tmp);
	while (diff-- > 0)
		go_left(line);
}
