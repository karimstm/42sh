/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 13:33:18 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/25 13:07:59 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		expand_redirection(t_redirection *redir)
{
	t_list_simple_command	*list;

	while (redir)
	{
		list = malloc_list_simple_command();
		if (redir->word)
		{
			token_push(list, ft_strdup(redir->word), TOKEN_WORD);
			setup_expan(list);
			ft_strdel(&redir->word);
			if (list->node_count == 1)
				redir->word = quote_stripping(list->head->name);
			else
				redir->word = NULL;
		}
		free_list(list);
		redir = redir->next;
	}
}
