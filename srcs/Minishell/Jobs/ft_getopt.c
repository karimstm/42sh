/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 11:19:05 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 11:19:07 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** This function check if an option is within args
** 0 on failure & 1 on success
*/

int					ft_getopt(char **args, char *optstring, int *argc)
{
	int		i;
	int		flag;
	char	*tmp;

	i = 0;
	flag = -1;
	while ((tmp = args[i]))
	{
		if (*tmp && tmp[0] == '-')
		{
			tmp++;
			while (*tmp)
			{
				if (ft_strchr(optstring, *tmp))
					flag = *tmp;
				else
					return ('?');
				tmp++;
			}
		}
		else
			return (flag);
		*argc = ++i;
	}
	return (flag);
}
