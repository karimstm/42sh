/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 13:10:15 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/11 00:41:50 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

struct termios	*get_termios(void)
{
	static struct termios	term;

	return (&term);
}

void	sig_handler(int sig)
{
	char c = 0;
	ft_printf("\n"); // Move to a new line
    (void)sig;
	ioctl(0, TIOCSTI , &c);
	// We need to clear the readline buffer here
	// nad redisplay a new prompt
}

void		child_handler(int sig)
{
	(void)sig;
	ft_printf("\n");
}

void		signals(void)
{
	signal(SIGINT, sig_handler);
	// signal(SIGWINCH, clr_screen); we need to verify if the new readline works well when the window size changes
}