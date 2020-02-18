/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 14:10:10 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/15 14:30:18 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			ft_tcsetpgrp(int fd, pid_t pgrp_id)
{
	return (ioctl(fd, TIOCSPGRP, &pgrp_id));
}

pid_t		ft_tcgetpgrp(int fd)
{
	int pgrp;

	if (ioctl(fd, TIOCGPGRP, &pgrp) < 0)
		return ((pid_t)-1);
	return ((pid_t)pgrp);
}
