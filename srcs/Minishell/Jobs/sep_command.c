/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 12:11:18 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/21 19:08:56 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** I DON'T KNOW WHY I USED NON INTERACTIVE MODE HERE
** thus I used kind = getpid() != shell_pgid ? J_NON_INTERACTIVE : J_FOREGROUND
*/

void			seperator_handling(t_job_list *job_list,
								t_node *node, t_blt_line *blt_line)
{
	t_job_kind kind;

	kind = getpid() != g_shell_pgid ? J_NON_INTERACTIVE : J_FOREGROUND;
	if (LEFT(node))
	{
		if (node->spec.sep_op_command->kind == ';')
			execute_entry(job_list, LEFT(node), blt_line, kind);
		else if (node->spec.sep_op_command->kind == '&')
			execute_entry(job_list, LEFT(node), blt_line, J_BACKGROUND);
	}
	if (RIGHT(node))
	{
		if (node->spec.sep_op_command->kind == ';')
			execute_entry(job_list, RIGHT(node), blt_line, kind);
		else if (node->spec.sep_op_command->kind == '&')
			execute_entry(job_list, RIGHT(node), blt_line, J_BACKGROUND);
	}
}

void			and_or_foreground(t_job_list *job_list, t_node *node,
								t_blt_line *blt_line, t_job_kind kind)
{
	int		status;

	status = 0;
	if (LEFT_A(node))
	{
		execute_entry(job_list, LEFT_A(node), blt_line, kind);
		status = job_list->tail->proc_list->tail->status;
	}
	if (RIGHT_A(node))
	{
		if ((status == 0 && node->spec.and_or_command->kind == TOKEN_AND_IF)
			|| (status && node->spec.and_or_command->kind == TOKEN_OR_IF))
			execute_entry(job_list, RIGHT_A(node), blt_line, kind);
	}
}

void			and_or_background(t_job_list *job_list, t_node *node,
								t_blt_line *blt_line, t_job_kind kind)
{
	dummy_process(job_list, node, kind);
	execute_simple_command(job_list, blt_line);
}

void			and_or_handling(t_job_list *job_list, t_node *node,
							t_blt_line *blt_line, t_job_kind kind)
{
	if (kind == J_FOREGROUND || kind == J_NON_INTERACTIVE)
		and_or_foreground(job_list, node, blt_line, kind);
	else if (kind == J_BACKGROUND)
		and_or_background(job_list, node, blt_line, kind);
}
