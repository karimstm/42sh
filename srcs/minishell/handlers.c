/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 13:10:15 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/27 10:34:46 by amoutik          ###   ########.fr       */
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
	ft_printf("\n"); // Move to a new line
    (void)sig;
	ERRNO = EOTE;
	ioctl(0, TIOCSTI , &c);
	// We need to clear the readline buffer here
	// nad redisplay a new prompt
}

void				child_handler(int sig)
{
	(void)sig;
	ft_printf("\n");
}

void				signals(void)
{
	signal(SIGINT, sig_handler);
	// signal(SIGWINCH, clr_screen); we need to verify if the new readline works well when the window size changes
}
