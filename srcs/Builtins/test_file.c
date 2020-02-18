/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 13:48:50 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/18 17:14:25 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			file_checked(struct stat *buf, int flag)
{
	if (flag == S_IFBLK)
		return (!!S_ISBLK(buf->st_mode));
	if (flag == S_IFCHR)
		return (!!S_ISCHR(buf->st_mode));
	if (flag == S_IFDIR)
		return (!!S_ISDIR(buf->st_mode));
	if (flag == S_IFREG)
		return (!!S_ISREG(buf->st_mode));
	if (flag == S_ISGID)
		return (!!(buf->st_mode & S_ISGID));
	if (flag == S_IFLNK)
		return (!!S_ISLNK(buf->st_mode));
	if (flag == S_IFIFO)
		return (!!S_ISFIFO(buf->st_mode));
	if (flag == S_IRUSR)
		return (!!(buf->st_mode & S_IRUSR));
	if (flag == S_IFSOCK)
		return (!!S_ISSOCK(buf->st_mode));
	if (flag == S_ISUID)
		return (!!(buf->st_mode & S_ISUID));
	if (flag == S_IWUSR)
		return (!!(buf->st_mode & S_IWUSR));
	if (flag == S_IXUSR)
		return (!!(buf->st_mode & S_IXUSR));
	return (-1);
}

int			do_file_checking(char *filename, int flag)
{
	struct stat	buf;

	if (filename == NULL)
		return (0);
	if (flag == 'n')
		return (ft_strlen(filename) > 0);
	if (flag == 'z')
		return (!ft_strlen(filename));
	if (lstat(filename, &buf) == -1)
		return (-1);
	if (flag == 'e')
		return (1);
	if (flag == 's')
		return (buf.st_size > 0);
	return (file_checked(&buf, flag));
}
