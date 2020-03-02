/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 13:10:15 by amoutik           #+#    #+#             */
/*   Updated: 2020/03/02 18:38:01 by aait-ihi         ###   ########.fr       */
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
