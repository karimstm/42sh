/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_content_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/21 11:48:05 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/21 11:59:11 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void free_content_list(t_list **list, size_t size)
{
    t_list *tmp;

    (void)size;
    while (*list)
    {
       tmp = *list;
       list = &(*list)->next;
       free(tmp->content);
       free(tmp);
    }
}