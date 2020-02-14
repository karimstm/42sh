/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 18:24:07 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/14 16:20:13 by amoutik          ###   ########.fr       */
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

void		init_shell_variables()
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

int			sh_system(char *name, char save_history)
{
	t_node			*node;
	char			*history_cmd;
	char			*backup_line;

	if (ft_str_isnull(name))
		return(1);
	ERRNO = 0;
	history_cmd = NULL;
	backup_line = (char *)g_token.line;
	if ((node = start_parsing_command(name)))
	{
		push_to_stack(STACK_LIST, node);
		history_cmd = g_token.current;
		execute(JOB_LIST, node, get_set_blt(NULL));
		if (save_history)
			add_to_history(history_cmd, ft_strlen(history_cmd), 0);
		job_notification(JOB_LIST);
		free_stacked_node(STACK_LIST, JOB_LIST);
		if (history_cmd != g_token.current)
			ft_strdel(&history_cmd);
	}
	else
		return (1);
	ft_strdel(&g_token.current);
	if (backup_line != g_token.line)
		ft_strdel(&backup_line);
	ft_strdel((char **)&g_token.line);
	return (JOB_LIST->status);
}
