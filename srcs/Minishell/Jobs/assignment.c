/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 12:39:33 by amoutik           #+#    #+#             */
/*   Updated: 2020/03/01 16:05:23 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char					**get_assignements(t_list_simple_command *list)
{
	char				**assigns;
	int					assign_count;
	t_simple_command	*current;
	int					i;

	assigns = NULL;
	assign_count = 0;
	current = list && list->node_count ? list->head : NULL;
	while (current && current->kind == TOKEN_ASSIGNMENT_WORD)
	{
		assign_count++;
		current = current->next;
	}
	if (assign_count)
	{
		current = list && list->node_count ? list->head : NULL;
		assigns = (char **)ft_memalloc(sizeof(char*) * (assign_count + 1));
		i = 0;
		while (current && current->kind == TOKEN_ASSIGNMENT_WORD)
		{
			assigns[i++] = ft_strdup(current->name);
			current = current->next;
		}
	}
	return (assigns);
}

void					handle_assigns(t_list_simple_command *node_cmd)
{
	char				**assign;
	t_simple_command	*cmd;
	t_variables_list	*tmp;

	tmp = NULL;
	setup_expan(node_cmd);
	cmd = get_command_name(node_cmd);
	assign = get_assignements(node_cmd);
	if (assign && cmd)
	{
		tmp = env2;
		env2 = dup_env();
	}
	ft_set_var(assign, cmd);
}