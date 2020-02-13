/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/08 02:12:44 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/10 20:49:32 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void const *content, size_t size)
{
	t_list *lst;

	lst = NULL;
	if (content)
	{
		if (!(lst = (t_list *)ft_memalloc(sizeof(t_list))))
			return (NULL);
		if (!(lst->content = malloc(size)))
		{
			free(lst);
			return (NULL);
		}
		ft_memcpy(lst->content, content, size);
		lst->content_size = size;
	}
	return (lst);
}