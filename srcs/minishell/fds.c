/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 13:51:21 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 13:51:23 by amoutik          ###   ########.fr       */
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
