/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 23:23:56 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/03/01 19:33:56 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"

void	update_o_v_cursor(t_readline *env)
{
	const t_point	o_c = env->o_cursor;
	const t_point	o_v = env->ov_cursor;
	int				y;

	get_cursor_position(env);
	y = (o_v.x + env->cursor) / env->col;
	env->o_cursor = (t_point){o_c.x, o_c.y + (env->o_cursor.y - (o_v.y + y))};
	set_virtual_origin(env);
}

void	update_o_cursor(t_readline *env)
{
	int			diff;
	int			count;
	int			i;
	const int	*details = env->line_props.details;
	const int	lc = env->line_props.linecount - 1;

	i = 0;
	count = !!((details[i] + env->o_cursor.x + (i == lc)) % env->col);
	count += ((details[i] + env->o_cursor.x + (i == lc)) / env->col);
	i++;
	while (i < env->line_props.linecount)
	{
		count += !!((details[i] + (i == lc)) % env->col);
		count += ((details[i] + (i == lc)) / env->col);
		i++;
	}
	diff = (count + env->o_cursor.y) - env->row;
	if (diff > 0)
	{
		env->o_cursor.y -= diff;
		set_virtual_origin(env);
	}
}

void	get_cursor_position(t_readline *readline)
{
	int		column;
	int		row;
	char	buf;

	column = 0;
	row = 0;
	tputs("\e[6n", 1, ft_putchar);
	while (read(0, &buf, 1) > 0 && buf != '[')
		;
	while (read(0, &buf, 1) > 0 && buf != ';')
		if (ft_isdigit(buf))
			row = (row * 10) + (buf - '0');
	while (read(0, &buf, 1) > 0 && buf != 'R')
		if (ft_isdigit(buf))
			column = (column * 10) + (buf - '0');
	readline->o_cursor = (t_point){column - 1, row - 1};
	readline->ov_cursor = readline->o_cursor;
}

char	*remove_unprintable_chars(char *str)
{
	char	*ret;
	int		i;

	ret = str;
	i = 0;
	if (str)
	{
		while (*str)
		{
			if (ft_isprint(*str) || *str == '\n')
			{
				ret[i++] = *str;
			}
			str++;
		}
		ret[i] = 0;
	}
	return (ret);
}

int		put_char(int c)
{
	ft_putchar(c);
	return (0);
}
