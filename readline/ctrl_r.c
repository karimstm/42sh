/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_r.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 06:03:16 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/11 06:03:18 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"

static char	*g_original = NULL;

static void	print_cmd(t_readline *line, char *format, char *str)
{
	tputs(tgetstr("vi", 0), 0, put_char);
	tputs(tgoto(tgetstr("cm", 0), line->o_cursor.x, line->o_cursor.y), 0,
			put_char);
	tputs(tgoto(tgetstr("LE", 0), 0, line->o_cursor.x), 0, put_char);
	tputs(tgetstr("cd", 0), 0, put_char);
	if (str)
		ft_printf(format, str, line->g_prompt);
	else
		ft_printf(format, line->g_prompt);
	get_cursor_position(line);
	set_cur_history(line, line->cmd);
	tputs(tgetstr("ve", 0), 0, put_char);
}

static void	change_cmd(t_readline *line, char *cmd, char *search, char *format)
{
	char	*tmp;

	tmp = line->cmd->tmp_line;
	line->cmd->tmp_line = ft_strdup(cmd);
	line->cmd->tmp_len = ft_strlen(cmd);
	free(tmp);
	print_cmd(line, format, search);
}

static void	search_cmd(int buff, t_readline *line, char **search)
{
	static t_cmd_history	*history = NULL;
	static int				not_found = 0;

	if (buff != -1 && buff != -2)
		*search = ft_strjoin_free(*search, (const char*)&buff);
	if (buff != -2 || (not_found == 0 && history == NULL))
		history = get_cmd_history_head();
	while (history)
	{
		not_found = 0;
		if (!ft_str_isnull(*search) && ft_strstr(history->tmp_line ?
							history->tmp_line : history->line, *search))
		{
			change_cmd(line, history->tmp_line, *search, FOUND_MSG);
			history = history->next;
			return ;
		}
		history = history->next;
		if (buff == -2 && history == NULL)
			history = get_cmd_history_head();
	}
	history = NULL;
	change_cmd(line, g_original, *search, !ft_strlen(*search) ? "'%s_' %s" :
															NOT_FOUND_MSG);
	not_found = 1;
}

static int	handle_keys(int buff, t_readline *line, char **search)
{
	int	r;

	r = 1;
	if (buff == BUTTON_ENTER && !(r = 0))
	{
		free(g_original);
		g_original = ft_strdup(line->cmd->tmp_line);
		ioctl(0, TIOCSTI, "\x1B");
	}
	else if (buff == BUTTON_CTL_R && !(r = 0) && ft_strlen(*search) > 0)
		search_cmd(-2, line, search);
	else if (buff == BUTTON_DEL && !(r = 0) && ft_strlen(*search) > 0)
	{
		search[0][ft_strlen(*search) - 1] = '\0';
		search_cmd(-1, line, search);
	}
	return (r);
}

void		handle_ctrl_r(t_readline *line)
{
	int				buff;
	int				max;
	char			*search;
	struct winsize	ws;

	g_original = ft_strdup(line->cmd->tmp_line);
	max = line->o_cursor.x + 15;
	print_cmd(line, "'_' %s", NULL);
	buff = 0;
	search = NULL;
	while (read(0, &buff, 4) != 0)
	{
		ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
		if (!ft_isprint(buff) && buff != BUTTON_ENTER
				&& buff != BUTTON_DEL && buff != BUTTON_CTL_R)
			break ;
		else if (handle_keys(buff, line, &search) == 1 &&
				(ws.ws_col >= ft_strlen(search) + max))
			search_cmd(buff, line, &search);
		buff = 0;
	}
	change_cmd(line, g_original, NULL, "%s");
	free(search);
	free(g_original);
	g_original = NULL;
}
