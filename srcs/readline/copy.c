/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 18:34:39 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/04/22 12:18:17 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	before(t_line *line, char mode)
{
	char	tmp;
	int		len;

	tmp = line->command[line->index + 1];
	line->command[line->index + 1] = '\0';
	len = ft_strlen(line->command);
	ft_strdel(&line->copy);
	line->copy = ft_strnew(len);
	ft_strcpy(line->copy, line->command);
	line->command[line->index + 1] = tmp;
	if (mode == MODE_CUT)
	{
		while (line->index >= 0)
			move_cursor(BACK_KEY, line);
	}
}

static void	after(t_line *line, char mode)
{
	int		len;

	len = ft_strlen(line->command + line->index + 1);
	ft_strdel(&line->copy);
	line->copy = ft_strnew(len);
	ft_strcpy(line->copy, line->command + line->index + 1);
	if (mode == MODE_CUT)
	{
		tputs(tgetstr("cd", NULL), 1, ft_putchar);
		line->top = line->index;
	}
}

static void	all(t_line *line, char mode)
{
	int		len;

	len = ft_strlen(line->command);
	ft_strdel(&line->copy);
	line->copy = ft_strnew(len);
	ft_strcpy(line->copy, line->command);
	if (mode == MODE_CUT)
	{
		go_home(line);
		tputs(tgetstr("cd", NULL), 1, ft_putchar);
		free_line();
		init_line();
	}
}

void		handle_copy(t_line *line, int key)
{
	if (key == CTRL_K)
		after(line, MODE_CUT);
	else if (key == CTRL_U)
		before(line, MODE_CUT);
	else if (key == CTRL_X)
		all(line, MODE_CUT);
	else if (key == ALT_RIGHT)
		after(line, MODE_COPY);
	else if (key == ALT_LEFT)
		before(line, MODE_COPY);
	else if (key == ALT_C)
		all(line, MODE_COPY);
}
