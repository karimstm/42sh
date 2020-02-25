/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_red.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 13:04:55 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/25 12:42:25 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			open_failed(char *name)
{
	ft_printf_fd(2, "42sh: %s: No such file or directory\n", name);
	return (1);
}

int			output(t_redirection *redir)
{
	const int	oflag = O_WRONLY | O_CREAT | O_TRUNC;

	if (check_file_permission(redir->word, W_OK))
	{
		if ((redir->fd2 = open(redir->word, oflag, 0644)) != -1)
			dup2(redir->fd2, redir->fd1);
		else
			return (open_failed(redir->word));
		if (redir->fd2 != redir->fd1)
			close(redir->fd2);
		return (0);
	}
	return (1);
}

int			output_append(t_redirection *redir)
{
	const int	oflag = O_WRONLY | O_CREAT | O_APPEND;

	if (check_file_permission(redir->word, W_OK))
	{
		if ((redir->fd2 = open(redir->word, oflag, 0644)) != -1)
			dup2(redir->fd2, redir->fd1);
		else
			return (open_failed(redir->word));
		if (redir->fd2 != redir->fd1)
			close(redir->fd2);
		return (0);
	}
	return (1);
}

int			dashed_aggregate(t_redirection *redir, int is_append)
{
	if (redir->word && redir->word[0] == '-')
	{
		if (redir->fd2 == -1)
			close(redir->fd1);
		return (0);
	}
	else if (check_file_permission(redir->word, W_OK))
	{
		DECLARE(int, _(oflag, O_WRONLY | O_CREAT));
		if (!is_append)
			redir->fd2 = open(redir->word, oflag | O_TRUNC, 0644);
		else
			redir->fd2 = open(redir->word, oflag | O_APPEND, 0644);
	}
	else
		return (1);
	return (-1);
}

int			dup4(int fd1, int fd2)
{
	dup2(fd1, fd2);
	close(fd2);
	return (0);
}

int			output_with_aggregate(t_redirection *redir, int is_append)
{
	int		status;

	if (redir->fd2 < 0)
	{
		if ((status = dashed_aggregate(redir, is_append)) != -1)
			return (status);
	}
	else
	{
		if (redir->word && redir->word[0] == '-')
		{
			if (fd_is_valid(redir->fd2))
				return (dup4(redir->fd1, redir->fd2));
			else
				return (bad_fd(redir->fd2));
		}
	}
	if (fd_is_valid(redir->fd2))
	{
		redir->fd1 == 1 ? dup2(redir->fd2, 2) : 0;
		dup2(redir->fd2, redir->fd1);
		return (0);
	}
	else
		return (bad_fd(redir->fd2));
}
