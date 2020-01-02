/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_string.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 09:16:17 by amoutik           #+#    #+#             */
/*   Updated: 2019/05/07 09:35:44 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"

void	free_token(t_token *node)
{
	ft_strdel(&node->token);
	free(node);
}

int		ret_with_str_free(t_string *str, int ret)
{
	free_string(str);
	return (ret);
}

void	free_tokens(t_token_list *list)
{
	t_token *current;
	t_token *tmp;

	current = list->head;
	while (current)
	{
		ft_strdel(&current->token);
		tmp = current->next;
		free(current);
		current = tmp;
	}
	list->tail = NULL;
	list->head = NULL;
	list->node_count = 0;
}

void	free_string(t_string *str)
{
	ft_strdel(&str->string);
}
