/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:13:55 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/25 14:48:07 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** print redirection list
*/

void			free_list(t_list_simple_command *list)
{
	t_simple_command *current;
	t_simple_command *tmp;

	current = NULL;
	if (list && list->head)
		current = list->head;
	while (current)
	{
		ft_strdel(&current->name);
		tmp = current->next;
		free(current);
		current = tmp;
	}
	free(list);
	list = NULL;
}

void			print_redir(t_redirection *list)
{
	t_redirection *current;

	current = list;
	while (current)
	{
		if (current->fd1 >= 0)
			ft_printf("%d", current->fd1);
		ft_printf("%s", redirect_name(current->kind));
		if (current->word)
			ft_printf("%s", current->word);
		else
			ft_printf("%d", current->fd2);
		current = current->next;
	}
}

void			free_redir(t_redirection **redir)
{
	t_redirection	*current;
	t_redirection	*tmp;

	current = *redir;
	while (current)
	{
		tmp = current->next;
		ft_strdel(&current->word);
		free(current);
		current = tmp;
	}
	*redir = NULL;
}

void			free_semi_node(t_node *current)
{
	if (current->spec.sep_op_command->left)
		free_tree(&current->spec.sep_op_command->left);
	if (current->spec.sep_op_command->right)
		free_tree(&current->spec.sep_op_command->right);
	free(current->spec.sep_op_command);
}

void			free_tree(t_node **node)
{
	t_node *current;

	current = *node != NULL ? *node : NULL;
	if (current)
	{
		if (current->redir)
			free_redir(&current->redir);
		if (current->kind == NODE_SEMI_AND)
			free_semi_node(current);
		if (current->kind == NODE_AND_OR || current->kind == NODE_PIPE)
		{
			if (current->spec.and_or_command->left)
				free_tree(&current->spec.and_or_command->left);
			if (current->spec.and_or_command->right)
				free_tree(&current->spec.and_or_command->right);
			free(current->spec.and_or_command);
		}
		else if (current->kind == NODE_SIMPLE_COMMAND)
			free_list(current->spec.simple_command);
		free(current);
		current = NULL;
	}
	*node = NULL;
}
