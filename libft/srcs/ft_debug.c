/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_debug.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 01:42:15 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/03/02 11:00:43 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_debug(char *tty, const char *format, ...)
{
	int		fd;
	va_list	ap;

	fd = open(tty, 0x0002);
	va_start(ap, format);
	ft_vprintf(fd, format, &ap);
	va_end(ap);
	close(fd);
}
