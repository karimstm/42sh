/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstenqueue.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 00:44:52 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/06 20:08:48 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

t_list	*ft_lstenqueue(t_list **list, t_list *new)
{
	if (new && list && *list)
	{
		if ((*list)->tail)
			(*list)->tail->next = new;
		else
			(*list)->next = new;
		(*list)->tail = new;
		return ((*list));
	}
	else if (new && list)
	{
		*list = new;
		return ((*list));
	}
	return (NULL);
}
