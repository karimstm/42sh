/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 18:24:07 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/13 19:21:22 by amoutik          ###   ########.fr       */
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

int			sh_system(char *name)
{
	t_node			*node;

	if (ft_str_isnull(name))
		return(1);
	ERRNO = 0;
	if ((node = start_parsing_command(name)))
	{
		push_to_stack(STACK_LIST, node);
		add_to_history(g_token.current, ft_strlen(g_token.current));
		execute(JOB_LIST, node, get_set_blt(NULL));
		job_notification(get_job_list(NULL));
		free_stacked_node(STACK_LIST, JOB_LIST);
	}
	else
		return (1);
	return (0);
}
