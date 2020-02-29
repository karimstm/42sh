/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 13:51:21 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/29 12:11:46 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	set_fds(int tmp_stds[3])
{
	tmp_stds[0] = dup3(0);
	tmp_stds[1] = dup3(1);
	tmp_stds[2] = dup3(2);
}

int		dup3(int oldfd)
{
	if (fcntl(oldfd, F_GETFD) < 0)
		return (-1);
	return (fcntl(oldfd, F_DUPFD, 100));
}

void	restore_fds(int std[2])
{
	dup2(std[0], 0);
	dup2(std[1], 1);
}

int		dup4(int fd1, int fd2)
{
	dup2(fd1, fd2);
	close(fd2);
	return (0);
}
