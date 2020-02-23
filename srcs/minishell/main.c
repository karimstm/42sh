/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 10:57:10 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/22 16:43:14 by amoutik          ###   ########.fr       */
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

t_node			*start_parsing_command(const char *line)
{
	t_node *node;

	node = NULL;
	init_stream(line);
	node = parse_commands();
	if (g_token.kind != TOKEN_EOF && !ERRNO)
		unexpected_error();
	if (ERRNO)
	{
		free_tree(&node);
		node = NULL;
	}
	return (node);
}

void			free_stacked_node(t_stack *sp, t_job_list *jobs)
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

void			run_shell2(t_list *blt)
{
	t_node			*node;
	char			*new_line;
	char			*tmp;

	(void)blt;
	init_shell_variables();
	while ((tmp = ft_readline(MSG)))
	{
		ERRNO = 0;
		new_line = pre_parse(tmp);
		ft_strdel(&tmp);
		job_notification(JOB_LIST);
		node = NULL;
		init_shell();
		sh_system(new_line, 1);
		init_shell();
		ft_strdel(&new_line);
	}
	deallocate(STACK_LIST);
	free(STACK_LIST);
	ft_printf("%s", WRONG_READ);
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
	t_fifo_list	*list;

	(void)ac;
	(void)av;
	list = (t_fifo_list *)xmalloc(sizeof(t_fifo_list));
	init_fifo_list(list);
	fifo_list(list);
	blt = NULL;
	env = NULL;
	history = NULL;
	signals();
	ft_init_env(ev);
	init_builtin(&blt);
	init_shell();
	restore_history();
	run_shell2(blt);
	free_builtin(blt);
	return (0);
}
