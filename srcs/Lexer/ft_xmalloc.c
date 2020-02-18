/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_xmalloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 15:55:48 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 15:55:49 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void					*xmalloc(size_t num_bytes)
{
	void *ptr;

	ptr = malloc(num_bytes);
	if (!ptr)
	{
		ft_printf_fd(2, "xmalloc failed");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}
