/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spliter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 09:29:20 by amoutik           #+#    #+#             */
/*   Updated: 2019/05/08 17:46:42 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	is_special_char(char c)
{
	if (c == '<' || c == '>' || c == '&')
		return (1);
	return (0);
}

/*
** Here we check if our line command end in proper way or not
*/

int	is_pipe_at_end(t_token_list *tokens)
{
	t_token *current;

	current = tokens->head;
	if (tokens->tail == NULL)
		return (0);
	if (tokens->tail == tokens->head)
		return (0);
	while (current)
	{
		if (current->next == tokens->tail)
		{
			if (current->tok_type == SH_REDIRECTION
					&& tokens->tail->tok_type == SH_PIPE)
				return (0);
			if (tokens->tail->tok_type & SH_PIPE
					|| tokens->tail->tok_type & SH_DPIPE)
				return (1);
		}
		current = current->next;
	}
	return (0);
}
