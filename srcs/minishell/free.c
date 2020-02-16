/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:49:45 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/16 12:49:15 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
**	called from exec_cmd,
**	it needed just because of the norm of 42,
**	there is no more line left in the exec_cmd
**	it frees the 3 elements given as parametes.
*/

void	free_exec_cmd(char *error, char *full_path, char **head_path)
{
	free(error);
	free(full_path);
	ft_free_strtab(head_path);
}
