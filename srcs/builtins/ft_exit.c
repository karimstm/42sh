/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 13:44:52 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/29 13:44:55 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	ft_exit(char **cmds)
{
	int status;

	status = 0;
	if (cmds && cmds[0])
		status = ft_atoi(cmds[0]) % 255;
	add_to_history(g_token.current, ft_strlen(g_token.current), 0);
	save_history();
	free_history();
	exit(status);
}
