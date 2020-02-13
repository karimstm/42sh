/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc_s.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 06:03:16 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/11 06:03:18 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	fc_exec(char *rep, char *cmd)
{
	(void)rep;
	(void)cmd;
}

void		fc_s(char *first, char *last)
{
	char	*rep;
	char	*cmd;

	rep = NULL;
	cmd = NULL;
	if (ft_strchr(first, '=') != NULL)
	{
		rep = first;
		cmd = last;
	}
	else
		cmd = first;
	fc_exec(rep, cmd);
}
