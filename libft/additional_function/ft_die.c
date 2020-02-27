/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_die.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/03 02:04:56 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/25 15:52:34 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_die(const char *message, int error_nb)
{
	if (error_nb != 0 && message)
		ft_printf_fd(2,"%s", message);
	else if (message)
		ft_printf("%s", message);
	exit(error_nb);
}
