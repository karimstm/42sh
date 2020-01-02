/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/02 15:12:12 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/05/11 17:21:57 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
**	this function called from exec_cmd,
**	if the file not found or it's permission is denied,
**	this function if the string error not null, the function prints
**	the string error, otherwise it prints a msg Command not found...
*/

void	print_error(char *error)
{
	if (error != NULL)
	{
		ft_printf_fd(2, error);
		free(error);
	}
}

void	run_redirection_with_errors(char *error, t_token *node, int std[2])
{
	char	status;
	int		tmp[3];
	int		fd_backup;

	tmp[0] = dup(0);
	tmp[1] = dup(1);
	tmp[2] = dup(2);
	dup2(std[0], 0);
	dup2(std[1], 1);
	fd_backup = -1;
	if ((status = handle_redirection(node, &fd_backup)) != 0)
		handle_errors(status, 0);
	print_error(error);
	if (fd_backup != -1 && fd_backup != -3)
		close(fd_backup);
	restore_std(tmp);
}
