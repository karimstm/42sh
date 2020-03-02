/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 13:44:52 by amoutik           #+#    #+#             */
/*   Updated: 2020/03/02 19:10:52 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	ft_exit(char **cmds)
{
	int status;

	status = 0;
	if (cmds && cmds[0] && cmds[1])
		return (PRINT_ERROR2("exit", "too many arguments"));
	if (cmds && cmds[0] && !is_number(cmds[0]))
		return (PRINT_ERROR2("exit", "numeric argument required"));
	if (cmds && cmds[0])
		status = ft_atoi(cmds[0]) % 256;
	add_to_history(g_token.current, ft_strlen(g_token.current), 0);
	save_history();
	free_history();
	exit(status);
}
