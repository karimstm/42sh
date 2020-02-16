/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_rest.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:11:14 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 16:11:54 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void					print_list_tokens(t_list_simple_command *list)
{
	t_simple_command *current;

	current = NULL;
	if (list && list->head)
		current = list->head;
	while (current)
	{
		ft_printf("%s ", current->name);
		current = current->next;
	}
}

/*
**	SEPERATORS SEMI ';' AND '&'
*/

t_sep_op				*sep_commands(t_token_kind kind,
										t_node *left, t_node *right)
{
	t_sep_op *sep_cmd;

	sep_cmd = (t_sep_op *)xmalloc(sizeof(t_sep_op));
	sep_cmd->kind = kind;
	sep_cmd->left = left;
	sep_cmd->right = right;
	return (sep_cmd);
}

t_and_or				*and_or_commands(t_token_kind kind,
											t_node *left, t_node *right)
{
	t_and_or *and_or_cmd;

	and_or_cmd = (t_and_or *)xmalloc(sizeof(t_and_or));
	and_or_cmd->kind = kind;
	and_or_cmd->left = left;
	and_or_cmd->right = right;
	return (and_or_cmd);
}
