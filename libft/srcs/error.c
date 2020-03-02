/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:59:47 by amoutik           #+#    #+#             */
/*   Updated: 2020/03/02 11:01:32 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

int			error_func(const char *epath, int errno)
{
	ft_printf_fd(STDERR_FILENO, "error on file : %s\n", epath);
	return (errno);
}
