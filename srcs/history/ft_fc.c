/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 06:03:16 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/11 06:03:18 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	print_fc_usage(void)
{
	ft_printf("fc [-r] [-e editor] [first [last]]\n");
	ft_printf("fc -l [-nr] [first [last]]\n");
	ft_printf("fc -s [old=new] [first]\n");
}

static int	get_fc_flags(int flags, char *arg)
{
	arg++;
	while (*arg)
	{
		if (*arg == 'l')
			flags |= L_FLAG;
		else if (*arg == 'n')
			flags |= N_FLAG;
		else if (*arg == 'r')
			flags |= R_FLAG;
		else if (*arg == 's')
			flags |= S_FLAG;
		else
			return ((flags |= BREAK_FLAG));
		arg++;
	}
	return (flags);
}

static void	fc_exec(int flags, char *editor, char **args)
{
	char	*first;
	char	*last;

	first = *args;
	if (first != NULL)
		last = *(args + 1);
	else
		last = NULL;
	if ((flags & S_FLAG) > 0)
		fc_s(flags, first, last);
	else if ((flags & L_FLAG) > 0)
		fc_l(flags, first, last);
	else
		fc_edit(flags, editor, first, last);
}

int			ft_fc(char **args)
{
	char	*editor;
	int		flags;

	editor = NULL;
	flags = 0;
	while (*args)
	{
		if (ft_strncmp(*args, "-e", 2) == 0)
		{
			editor = ft_strlen(*args) == 2 ? *(++args) : *args + 2;
			if (editor == NULL)
			{
				print_fc_usage();
				return (1);
			}
		}
		else if (*args[0] != '-' || (*args[0] == '-' &&
				((flags = get_fc_flags(flags, *args)) & BREAK_FLAG) > 0))
			break ;
		args++;
	}
	fc_exec(flags, editor == NULL ? "vim" : editor, args);
	return (0);
}
