/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 10:57:10 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/05 09:09:10 by amoutik          ###   ########.fr       */
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

static int		search_semi(t_list *blt, t_list **env, t_token_list *tokens)
{
	t_token		*ptr;

	ptr = tokens->head;
	while (ptr)
	{
		if ((ptr->tok_type & SH_SEMI) != 0)
		{
			tokens->head = ptr->next;
			shell(blt, env, tokens);
		}
		ptr = ptr->next;
	}
	return (0);
}

/*
**  The loop function of minishell
**  it prints the minishell msg : My_Minishell $>
**  and reads from the input standard the command
**  and check it which type is it : exit, builtins,
**                                  Cmd with path, Cmd Without Path
**  and sends the list of agruments to appropriate function.
**  Notes : (libft functions)
**  get_next_line  : reads a line from the standard input.
**  ft_strsplit_ws : splits a line to a multiple words by whitespace delimiters
**           returns a char** and last pointer is NULL.
**  free_strtab    : frees all strings (char**) returned by ft_strsplit_ws.
*/

void		run_shell(t_list *blt, t_line *line)
{
	t_token_list	*tokens;
	t_token			*head;

	while (read_line(line) == 0)
	{
		while (1)
		{
			if (ft_str_isnull(line->command) ||
				(tokens = handle_quote(&line->command)) == NULL)
				break;
			m_add_history(line);
			if (parse_heredoc(tokens) != 0 && free_token_list(tokens))
				continue;
			head = tokens->head;
			shell(blt, &(line->env), tokens);
			search_semi(blt, &(line->env), tokens);
			tokens->head = head;
			free_token_list(tokens);
			break;
		}
		free_line();
		line = init_line();
	}
	ft_printf(WRONG_READ);
	free_line();
}

/*
** tmporary fix, it should be removed later on if it's not working ==========
*/
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

void		run_shell2(t_list *blt, t_line *line)
{
	t_node			*node;
	char			*new_line;
	t_job_list		*jobs;
	t_stack			sp;

	jobs = (t_job_list *)xmalloc(sizeof(t_job_list));
	init_stack(&sp, INIT_STACK_SIZE);
	init_job_list(jobs);
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
	init_env(&env, ev);
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
	free_env(env);
	free_builtin(blt);
	return (0);
}
