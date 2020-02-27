/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/21 16:01:25 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/26 11:56:15 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			skip_dqoute(char **string, char c)
{
	char	*new;
	size_t	len;

	new = *string;
	new++;
	while (*new)
	{
		if (*new == '\\' && c != '\'')
			new++;
		else if (*new == c)
		{
			new++;
			break ;
		}
		*new ? new++ : 0;
	}
	len = new - *string;
	*string = new;
	return (len);
}

pid_t		setup_proc(char *cmd, char *path, int flag)
{
	pid_t		child;
	int			output;
	int			fd;
	t_fifo_list	*list;

	output = flag == O_WRONLY ? dup3(1) : dup3(0);
	if ((child = fork()) == 0)
	{
		list = (t_fifo_list *)xmalloc(sizeof(t_fifo_list));
		init_fifo_list(list);
		fifo_list(list);
		(fd = open(path, flag)) == -1 ? exit(EXIT_FAILURE) : 0;
		flag == O_WRONLY ? dup2(fd, 1) : dup2(fd, 0);
		sh_system(cmd, 0);
		close_fifos(list);
		free(list);
		close(fd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		dup2(output, flag == O_WRONLY ? 1 : 0);
		close(output);
	}
	return (child);
}

char		*setup_fifo(void)
{
	char	*path;

	path = ft_tpname(NULL);
	if (!mkfifo(path, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH))
		return (path);
	return (NULL);
}

char		*scan_pro(char *name)
{
	size_t	len;
	char	*new;

	len = 0;
	new = NULL;
	DECLARE(int, _(index, -1), stack[100]);
	DECLARE(char, _(*start, name));
	while (*name)
	{
		if (*name == '"' || *name == '\'')
			skip_dqoute(&name, *name);
		if (*name == '(')
			stack[++index] = '(';
		else if (*name == ')')
			index--;
		if (index == -1)
			break ;
		name++;
	}
	len = name - start;
	if (index == -1 && len > 1)
		new = ft_strsub(++start, 0, len - 1);
	return (new);
}
