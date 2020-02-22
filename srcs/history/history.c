/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 06:03:16 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/13 12:31:53 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	restore_history(void)
{
	int		fd;
	int		first;

	DECLARE(char, _(*buf, NULL), _(*log_file, NULL));
	log_file = ft_strjoin(getenv("HOME"), "/.history");
	fd = open(log_file, O_RDONLY);
	free(log_file);
	if (fd == -1)
		return ;
	first = 1;
	while (get_next_line(fd, &buf) == 1)
	{
		if (buf[0] != '\n' && buf[0] != '\0')
			add_to_history(buf, ft_strlen(buf), 0);
		if (first)
		{
			first = 0;
			history_begining = get_cmd_history_head();
		}
		ft_strdel(&buf);
	}
	free_gnl(fd);
	new_history_begining = get_cmd_history_head();
	close(fd);
}

void	save_history(void)
{
	int		fd;
	char	*log_file;

	log_file = ft_strjoin(getenv("HOME"), "/.history");
	fd = open(log_file, O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	free(log_file);
	if (fd == -1)
		return ;
	while (new_history_begining)
	{
		ft_printf_fd(fd, "%s\n", new_history_begining->line);
		new_history_begining = new_history_begining->prev;
	}
	close(fd);
}

int		ft_history(char **args)
{
	t_cmd_history	*history;

	(void)args;
	history = history_begining;
	while (history)
	{
		ft_printf("%d\t%s\n", history->index, history->line);
		history = history->prev;
	}
	return (0);
}

char	*search_history(char *format)
{
	t_cmd_history	*history;

	history = NULL;
	if (format[0] == '!')
		history = get_cmd_history_head();
	else if (format[0] != '-' && !ft_isdigit(format[0]))
		history = get_specific_history_by_str(format, "");
	else if (format[0] == '-' && ft_isdigit(format[1]))
		history = get_history_by_reverse(ft_atoi(format + 1));
	else if (ft_isdigit(format[0]))
		history = get_specific_history(ft_atoi(format), 0);
	return (history ? history->line : NULL);
}
