/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_debug.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 01:42:15 by zoulhafi          #+#    #+#             */
/*   Updated: 2018/10/07 22:44:49 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void					ft_debug(char *tty, const char *format, ...)
{
	int		fd;

	fd = open(tty, 0x0002);
	va_list		ap;
	va_start(ap, format);
	ft_vprintf(fd, format, &ap);
	va_end(ap);
	close(fd);
}
