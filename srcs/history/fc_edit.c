/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc_edit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 06:03:16 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/11 06:03:18 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	write_cmds(int fd, char *first, char *last, int flags)
{
	t_cmd_history	*first_h;
	t_cmd_history	*last_h;
	t_cmd_history	*history;

	if (!(first_h = get_first(first)) ||
		!(last_h = get_last(last)))
		return ;
	if (first_h && last_h && last_h->index - first_h->index < 0 &&
			(flags |= R_FLAG) >= 0)
		ft_swap_pt((void *)&first_h, (void *)&last_h);
	history = first_h;
	if ((flags & R_FLAG) > 0)
		history = last_h;
	while (history)
	{
		ft_printf_fd(fd, "%s\n", history->line);
		if (((flags & R_FLAG) > 0 && history == first_h) ||
			((flags & R_FLAG) <= 0 && history == last_h))
			break ;
		history = (flags & R_FLAG) > 0 ? history->next : history->prev;
	}
}

int	fc_edit(int flags, char *editor, char *first, char *last)
{
	int		fd;
	int		status;
	char	*cmd;
	char	*rand_string;
	char	*path;

	rand_string = ft_tmpfile();
	path = ft_strjoin("/tmp/", rand_string);
	ft_strdel(&rand_string);
	fd = open(path, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	write_cmds(fd, first, last, flags);
	close(fd);
	cmd = ft_strjoin_pre(editor, " ", path);
	status = sh_system(cmd);
	ft_strdel(&path);
	ft_strdel(&cmd);
	ft_printf("status : %d\n", status);
	return (0);
}
