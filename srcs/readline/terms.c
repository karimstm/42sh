/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terms.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 10:34:16 by amoutik           #+#    #+#             */
/*   Updated: 2019/04/19 22:47:46 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

struct termios	*get_termios(void)
{
	static struct termios	term;

	return (&term);
}

int				init_termios(struct termios term)
{
	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &term) == -1)
		return (-1);
	return (1);
}

int				init_terms(void)
{
	char	*term;
	int		success;

	term = getenv("TERM");
	if (term == 0)
		return (-1);
	success = tgetent(NULL, term);
	if (success <= 0)
		return (-1);
	return (1);
}
