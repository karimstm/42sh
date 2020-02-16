/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 12:53:07 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 18:22:22 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			fd_is_valid(int fd)
{
	return (fcntl(fd, F_GETFD) != -1);
}

int			bad_fd(int fd)
{
	syntax_error("42sh: %d: Bad file descriptor", fd);
	return (1);
}

int			check_file_permission(char *filename, int perm)
{
	int error;

	error = 0;
	if (access(filename, F_OK) == 0)
	{
		if (access(filename, perm) == -1 && (error = 1))
			syntax_error("42sh: permission denied: %s", filename);
		else if (is_directory(filename) && (error = 1))
			syntax_error("42sh: %s: Is a Directory.", filename);
		return (!error ? 1 : 0);
	}
	return (1);
}

int			check_file_status(char *filename)
{
	int error;

	error = 1;
	if (is_directory(filename))
		syntax_error("42sh: %s: Is a Directory.", filename);
	else if (access(filename, F_OK) == -1)
		syntax_error("42sh: no such file or directory: %s", filename);
	else if (access(filename, R_OK) == -1)
		syntax_error("42sh: permission denied: %s", filename);
	else
		error = 0;
	return (error);
}
