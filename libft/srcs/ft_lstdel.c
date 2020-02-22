/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/08 02:47:00 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/22 00:38:41 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	t_list *cpy;
	t_list *next;

	if (alst == NULL || *alst == NULL || del == NULL)
		return;
	cpy = *alst;
	while (cpy->next != NULL)
	{
		next = cpy->next;
		(*del)(cpy->content, cpy->content_size);
		free(cpy);
		cpy = next;
	}
	(*del)(cpy->content, cpy->content_size);
	free(cpy);
	*alst = NULL;
}

void ft_lstdel2(t_list **alst, void (*del)(void *))
{
	t_list *to_free;

	if (alst && *alst && del)
	{
		while (*alst)
		{

			to_free = *alst;
			*alst = to_free->next;
			(*del)(to_free->content);
			free(to_free);
		}
	}
}