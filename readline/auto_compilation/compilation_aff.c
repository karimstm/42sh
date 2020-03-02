/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compilation_aff.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szakaria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 04:16:19 by szakaria          #+#    #+#             */
/*   Updated: 2020/03/01 04:17:55 by szakaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_compilation.h"

void		free_tab(char **stock)
{
	int	i;

	i = 0;
	while (stock[i])
		free(stock[i++]);
	free(stock);
	stock = NULL;
}

int			read_keybord(char **stock, int first)
{
	static int	move;
	int			buff;
	int			i;

	buff = 0;
	i = 0;
	if (!first)
		move = 0;
	while (stock[i])
		i++;
	read(0, &buff, sizeof(int));
	if (buff == BUTTON_UP && move)
		move--;
	else if (buff == BUTTON_DOWN && move < i - 1)
		move++;
	else if (buff == EXIT)
		return (-2);
	else if (buff == BUTTON_SPACE || buff == BUTTON_ENTER)
		return (-1);
	ft_putstr_fd(tgetstr("cl", NULL), 0);
	return (move);
}

void		affichage(char **stock, int i, int len)
{
	struct winsize	term;

	ioctl(0, TIOCGWINSZ, &term);
	while (stock[i])
	{
		if (len == term.ws_row - 1)
			break ;
		if (len == 0)
			ft_putstr_fd("\x1b[92m ➜ ", 0);
		else
			ft_putstr_fd("\x1b[31m ➜ ", 0);
		ft_putendl_fd(stock[i], 0);
		len++;
		i++;
	}
}

void		termcap_config(int mode)
{
	if (mode == 0)
	{
		ft_putstr_fd(tgetstr("ti", NULL), 0);
		ft_putstr_fd(tgetstr("vi", NULL), 0);
		ft_putstr_fd(tgetstr("cl", NULL), 0);
	}
	else if (mode == 1)
	{
		ft_putstr_fd(tgetstr("ve", NULL), 0);
		ft_putstr_fd(tgetstr("te", NULL), 0);
	}
}

char		*print_window_element(char **stock, int i)
{
	static int		first;
	static int		past;

	if (i == -2)
	{
		i = 0;
		past = 0;
		first = 0;
	}
	termcap_config(0);
	DECLARE(int, _(len, 0));
	affichage(stock, i, len);
	if ((i = read_keybord(stock, first)) >= 0)
	{
		first++;
		past = i;
		print_window_element(stock, i);
	}
	termcap_config(1);
	if (i == -2 || past == -2)
	{
		past = -2;
		return (NULL);
	}
	return (stock[past]);
}
