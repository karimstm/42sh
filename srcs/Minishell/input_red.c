/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_red.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 13:06:29 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/25 12:50:32 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** Input redirection part
*/

int		input(t_redirection *redir)
{
	if (check_file_status(redir->word) == 0)
	{
		if ((redir->fd2 = open(redir->word, O_RDONLY)) != -1)
		{
			dup2(redir->fd2, redir->fd1);
			if (redir->fd2 != redir->fd1)
				close(redir->fd2);
			return (0);
		}
	}
	return (1);
}

int		input_here_doc(t_redirection *redir)
{
	char	*path;

	path = ft_strjoin("/tmp/", redir->herdoc);
	if (check_file_status(path) == 0)
	{
		if ((redir->fd2 = open(path, O_RDONLY)))
		{
			dup2(redir->fd2, redir->fd1);
			if (redir->fd2 != redir->fd1)
				close(redir->fd2);
			return (0);
		}
	}
	ft_strdel(&path);
	return (1);
}

int		input_with_aggregate(t_redirection *redir)
{
	if (redir->fd2 < 0)
	{
		if (redir->word && redir->word[0] == '-')
		{
			if (redir->fd2 == -1)
				close(redir->fd1);
			return (0);
		}
	}
	else
	{
		if (redir->word && redir->word[0] == '-')
		{
			if (fd_is_valid(redir->fd2))
			{
				dup2(redir->fd1, redir->fd2);
				close(redir->fd2);
				return (0);
			}
			else
				return (bad_fd(redir->fd2));
		}
	}
	if (fd_is_valid(redir->fd2))
	{
		if (redir->fd1 == 1)
			dup2(redir->fd2, 2);
		dup2(redir->fd2, redir->fd1);
		return (0);
	}
	else
		return (bad_fd(redir->fd2));
}

int		input_output(t_redirection *redir)
{
	if (check_file_status(redir->word) == 0)
	{
		if ((redir->fd2 = open(redir->word, O_RDWR | O_CREAT, 0644)))
		{
			dup2(redir->fd2, redir->fd1);
			if (redir->fd2 != redir->fd1)
				close(redir->fd2);
			return (0);
		}
	}
	return (1);
}
