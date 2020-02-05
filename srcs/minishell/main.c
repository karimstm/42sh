/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 10:57:10 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/05 15:43:01 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void			restore_std(int std[3])
{
	dup2(std[0], 0);
	dup2(std[1], 1);
	dup2(std[2], 2);
	close(std[0]);
	close(std[1]);
	close(std[2]);
}

static t_node	*start_parsing_command(const char *line)
{
	t_node *node;

	node = NULL;
	init_stream(line);
	node = parse_commands();
	if (g_token.kind != TOKEN_EOF && !ERRNO)
		unexpected_error();
	if (ERRNO)
		free_tree(&node);
	return (node);
}

void		free_stacked_node(t_stack *sp, t_job_list *jobs)
{
	t_node *node;

	node = NULL;
	if (jobs->node_count == 0)
	{
		while (!is_underflow(sp))
		{
			node = pop_stack(sp);
			free_tree(&node);
		}
	}
}

//For debug only
// Should be removed later on
void		fake_alias()
{
	char *name[] = {"ec=echo \"", NULL};
	ft_alias(name);
}

void		run_shell2(t_list *blt, t_line *line)
{
	t_node			*node;
	char			*new_line;
	t_job_list		*jobs;
	t_alias_list	*aliases;
	t_stack			sp;

	jobs = (t_job_list *)xmalloc(sizeof(t_job_list));
	aliases = (t_alias_list *)xmalloc(sizeof(t_alias_list));
	init_stack(&sp, INIT_STACK_SIZE);
	init_job_list(jobs);
	init_alias(aliases);
	get_alias_list(aliases);
	fake_alias(); ////
	get_job_list(jobs);
	while ((new_line = readline(MSG)))
	{
		ERRNO = 0;
		job_notification(jobs);
		node = NULL;
		init_shell();
		if (ft_str_isnull(new_line) ||
			(node = start_parsing_command(new_line)) == NULL)
		{
			ft_strdel((char **)&g_token.line);
			continue;
		}
		push_to_stack(&sp, node);
		execute(jobs, node, line, blt);
		job_notification(jobs);
		free_stacked_node(&sp, jobs);
		ft_strdel((char **)&g_token.line);
	}
	deallocate(&sp);
	ft_printf(WRONG_READ);
}

void    ft_printenv()
{
    t_variables *cur;

    cur = env2->head;
    while (cur)
    {
        ft_printf("%d /// %s=%s\n",cur->is_exported, cur->key, cur->value);
        cur = cur->next;
    }
}

/*
**	The Main Function of Minishell
**	it initiates the builtins and environment lists,
**	after calls the loop function of minishell,
**	after frees all memory allocated on the heap
*/

int				main(int ac, char **av, char **ev)
{
	t_list		*env;
	t_list		*blt;
	t_list		*history;
	t_line		*new_line;

	(void)ac;
	(void)av;
	blt = NULL;
	env = NULL;
	history = NULL;
	signals();
	ft_init_env(ev);
	init_builtin(&blt);
	init_shell();
	new_line = init_line();
	new_line->tail_history = &history;
	new_line->env = env;
	new_line->copy = NULL;
	run_shell2(blt, new_line);
	free(new_line->copy);
	free_gnl(0);
	free_builtin(blt);
	return (0);
}
