/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 09:53:31 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/03/02 21:07:19 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"

int		manage_ctlr_d(t_readline *readline)
{
	if (*readline->cmd->tmp_line == '\0')
	{
		ft_memdel((void **)&readline->cmd->tmp_line);
		free(readline->line_props.details);
		ft_strdel(&readline->to_past);
		clean_hsitory();
		unconfigure_terminal(readline);
		ft_putstr("\n");
		return (1);
	}
	return (0);
}

char	*manage_ctlr_c(t_readline *readline)
{
	const t_point	cursor = readline->o_cursor;

	tputs(tgoto(tgetstr("cm", 0), cursor.x, cursor.y), 0, put_char);
	if (readline->cmd && readline->cmd->tmp_line)
		ft_putendl(readline->cmd->tmp_line);
	ft_memdel((void **)&readline->cmd->tmp_line);
	free(readline->line_props.details);
	ft_strdel(&readline->to_past);
	clean_hsitory();
	unconfigure_terminal(readline);
	return (ft_strdup(""));
}

void	clear_buffer(t_readline *readline)
{
	readline->o_cursor.y = 0;
	set_virtual_origin(readline);
	tputs(tgoto(tgetstr("cm", 0), 0, readline->o_cursor.y), 0, put_char);
	update_o_cursor(readline);
	tputs(tgetstr("cd", NULL), 0, put_char);
	ft_printf("%s%s", readline->g_prompt, readline->cmd->tmp_line);
	cur_goto(readline, readline->cursor);
	rewrite_line(readline);
}

void	configure_terminal(t_readline *readline)
{
	if (tcgetattr(0, &readline->config) < 0)
		ft_die("Can't get terminal config", -10);
	readline->config.c_lflag &= ~(ECHO | ICANON);
	if (tcsetattr(0, 0, &readline->config) < 0)
		ft_die("Can't set terminal config", -11);
	if (tgetent(NULL, getenv("TERM")) < 1)
		ft_die("Can't initialize terminal config", -12);
}

void	unconfigure_terminal(t_readline *readline)
{
	readline->config.c_lflag |= (ECHO | ICANON);
	if (tcsetattr(0, 0, &readline->config) < 0)
		ft_die("Can't set terminal config", -11);
}
