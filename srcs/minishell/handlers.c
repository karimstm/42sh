/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 13:10:15 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/29 12:10:37 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

struct termios		*get_termios(void)
{
	static struct termios	term;

	return (&term);
}

void				sig_handler(int sig)
{
	char c;

	c = 0;
	ft_printf("\n");
	(void)sig;
	ERRNO = EOTE;
	ioctl(0, TIOCSTI, &c);
}

void				child_handler(int sig)
{
	(void)sig;
	ft_printf("\n");
}

void				signals(void)
{
	signal(SIGINT, sig_handler);
}
