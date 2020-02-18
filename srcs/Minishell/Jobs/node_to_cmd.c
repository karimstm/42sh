/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_to_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 11:17:53 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 12:24:00 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_job_list			*get_job_list(t_job_list *jobs)
{
	static t_job_list	*list = NULL;

	if (jobs != NULL)
		list = jobs;
	return (list);
}

void				print_semi_and(t_node *node)
{
	if (node->spec.sep_op_command->left)
		ft_print_node(node->spec.sep_op_command->left);
	ft_printf("%c", node->spec.sep_op_command->kind);
	if (node->spec.sep_op_command->right)
		ft_print_node(node->spec.sep_op_command->right);
}

void				print_and_or_pipe(t_node *node)
{
	if (node->spec.and_or_command->left)
		ft_print_node(node->spec.and_or_command->left);
	ft_printf("%s", token_name(node->spec.and_or_command->kind));
	if (node->spec.and_or_command->right)
		ft_print_node(node->spec.and_or_command->right);
}

void				ft_print_node(t_node *node)
{
	if (node)
	{
		if (node->kind == NODE_SEMI_AND)
			print_semi_and(node);
		else if (node->kind == NODE_AND_OR || node->kind == NODE_PIPE)
			print_and_or_pipe(node);
		else if (node->kind == NODE_SIMPLE_COMMAND)
		{
			ft_printf(" ");
			print_list_tokens(node->spec.simple_command);
		}
		print_redir(node->redir);
	}
}
