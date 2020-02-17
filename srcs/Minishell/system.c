/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 18:24:07 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/17 11:21:54 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_stack		*get_stack(t_stack *sp)
{
	static t_stack	*new_stack = NULL;

	if (sp != NULL)
		new_stack = sp;
	return (new_stack);
}

void		init_shell_variables(void)
{
	t_job_list		*jobs;
	t_alias_list	*aliases;
	t_stack			*sp;
	t_hash_table	*ht;

	sp = (t_stack *)xmalloc(sizeof(t_stack));
	jobs = (t_job_list *)xmalloc(sizeof(t_job_list));
	aliases = (t_alias_list *)xmalloc(sizeof(t_alias_list));
	ht = get_hash_table(ht_new());
	init_stack(sp, INIT_STACK_SIZE);
	init_job_list(jobs);
	init_alias(aliases);
	get_alias_list(aliases);
	get_job_list(jobs);
	get_stack(sp);
}

int			fc_found(t_list_simple_command *list)
{
	t_simple_command *cmd;

	cmd = list && list->head ? list->head : NULL;
	while (cmd)
	{
		if (cmd->kind == TOKEN_WORD)
		{
			if (ft_strequ(cmd->name, "fc"))
				return (1);
			else
				return (0);
		}
		cmd = cmd->next;
	}
	return (0);
}

int			find_fc(t_node *node, int *status)
{
	if (node)
	{
		if (node->kind == NODE_SIMPLE_COMMAND)
		{
			if (!*status && (*status = fc_found(node->spec.simple_command)))
				return (1);
		}
		else if (node->kind == NODE_AND_OR || node->kind == NODE_PIPE)
		{
			if (node->spec.and_or_command)
			{
				find_fc(node->spec.and_or_command->left, status);
				find_fc(node->spec.and_or_command->right, status);
			}
		}
		else if (node->kind == NODE_SEMI_AND)
		{
			if (node->spec.sep_op_command)
			{
				find_fc(node->spec.sep_op_command->left, status);
				find_fc(node->spec.sep_op_command->right, status);
			}
		}
	}
	return (*status);
}

int			sh_system(char *name, char save_history)
{
	t_node			*node;

	if (ft_str_isnull(name))
		return (1);
	ERRNO = 0;
	DECLARE(char, _(*history_cmd, NULL), _(*backup_line, (char *)g_token.line));
	DECLARE(int, _(status, 0));
	if ((node = start_parsing_command(name)))
	{
		push_to_stack(STACK_LIST, node);
		history_cmd = g_token.current;
		execute(JOB_LIST, node, get_set_blt(NULL));
		if (save_history && !find_fc(node, &status))
			add_to_history(history_cmd, ft_strlen(history_cmd), 0);
		job_notification(JOB_LIST);
		free_stacked_node(STACK_LIST, JOB_LIST);
		if (history_cmd != g_token.current)
			ft_strdel(&history_cmd);
	}
	ft_strdel(&g_token.current);
	if (backup_line != g_token.line)
		ft_strdel(&backup_line);
	ft_strdel((char **)&g_token.line);
	return (JOB_LIST->status);
}
