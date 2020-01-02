/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 08:42:05 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/03/17 21:41:24 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *buf, size_t size, size_t old_size)
{
	void	*new;

	while ((new = ft_memalloc(size)) == NULL)
		;
	ft_memcpy(new, buf, old_size);
	free(buf);
	return (new);
}
