/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xrealloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:52:47 by amoutik           #+#    #+#             */
/*   Updated: 2019/05/08 17:53:22 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*xrealloc(void *ptr, size_t size)
{
	char *new;

	if (size > ft_strlen((char *)ptr))
	{
		new = (char *)ft_memalloc(size);
		ft_memcpy(new, (char *)ptr, ft_strlen((char *)ptr));
		ft_memdel(&ptr);
		return (new);
	}
	return (NULL);
}
