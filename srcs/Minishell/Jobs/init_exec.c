/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 12:10:18 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/22 16:46:30 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void			simple_command(t_job_list *job_list,
							t_node *node, t_blt_line *blt_line, t_job_kind kind)
{
	char				**assign;
	char				*name;
	t_simple_command	*cmd;
	t_variables_list	*tmp;

	tmp = NULL;
	setup_expan(node->spec.simple_command);
	cmd = get_command_name(node->spec.simple_command);
	name = cmd && cmd->name ? ft_strdup(cmd->name) : NULL;
	assign = get_assignements(node->spec.simple_command);
	dummy_process(job_list, node, kind);
	if (assign)
	{
		if (cmd)
		{
			tmp = env2;
			env2 = dup_env();
		}
		ft_set_var(assign);
	}
	execute_simple_command(job_list, blt_line);
	if (name && cmd && cmd->type == IS_FOUND)
		ht_insert(get_hash_table(NULL), name, cmd->name);
	reset_env(tmp, assign);
	ft_strdel(&name);
}

void			consume_pipe(t_list_process *pro_list, t_node *node)
{
	if (node)
	{
		if (node->kind == NODE_PIPE)
		{
			if (node->spec.and_or_command->left)
				consume_pipe(pro_list, node->spec.and_or_command->left);
			if (node->spec.and_or_command->right)
				consume_pipe(pro_list, node->spec.and_or_command->right);
		}
		else
			process_push(pro_list, 0, node);
	}
}

void			pipe_entry(t_job_list *job_list,
						t_node *node, t_blt_line *blt_line, t_job_kind kind)
{
	t_list_process		*pro_list;

	pro_list = (t_list_process *)xmalloc(sizeof(t_list_process));
	init_process_list(pro_list);
	consume_pipe(pro_list, node);
	job_push(job_list, pro_list, 0);
	job_list->tail->kind = kind;
	execute_simple_command(job_list, blt_line);
}

void			execute_entry(t_job_list *job_list, t_node *node,
							t_blt_line *blt_line, t_job_kind kind)
{
	int					tmp[3];

	if (node && node->redir && kind == J_FOREGROUND
		&& node->kind != NODE_SIMPLE_COMMAND)
	{
		set_fds(tmp);
		execute_redirection(reverse_redirection(node->redir));
	}
	if (node)
	{
		if (node->kind == NODE_SEMI_AND)
			seperator_handling(job_list, node, blt_line);
		else if (node->kind == NODE_AND_OR)
			and_or_handling(job_list, node, blt_line, kind);
		else if (node->kind == NODE_PIPE)
			pipe_entry(job_list, node, blt_line, kind);
		else if (node->kind == NODE_SIMPLE_COMMAND)
			simple_command(job_list, node, blt_line, kind);
	}
	if (node && node->redir && kind == J_FOREGROUND
		&& node->kind != NODE_SIMPLE_COMMAND)
		restore_std(tmp);
}

void			execute(t_job_list *job_list, t_node *node, t_list *blt)
{
	t_job_kind	kind;
	t_blt_line	*blt_line;

	kind = J_FOREGROUND;
	blt_line = (t_blt_line *)xmalloc(sizeof(t_blt_line));
	blt_line->blt = blt;
	execute_entry(job_list, node, blt_line, kind);
	free(blt_line);
}
