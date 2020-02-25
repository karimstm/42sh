/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 13:52:49 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/25 12:30:34 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void			restore_fds(int std[2])
{
	dup2(std[0], 0);
	dup2(std[1], 1);
}

char			**node_to_char(t_list_simple_command *command)
{
	char				**cmds;
	int					i;
	t_simple_command	*current;

	current = (command && command->head) ? command->head : NULL;
	if (current == NULL || command->node_count == 0)
		return (NULL);
	cmds = (char **)xmalloc(sizeof(char *) * (command->node_count + 1));
	i = 0;
	while (current && current->kind != TOKEN_WORD)
		current = current->next;
	while (current)
	{
		cmds[i++] = quote_stripping(current->name);
		current = current->next;
	}
	cmds[i] = NULL;
	return (cmds);
}

char			*working_path(char *cmd)
{
	char		**all_paths;
	char		**tmp;
	t_variables	*var;
	char		*full_path;

	var = get_var("PATH");
	all_paths = var ? ft_strsplit(var->value, ':') : NULL;
	tmp = all_paths;
	full_path = NULL;
	while (*tmp)
	{
		full_path = ft_strjoin_pre(*tmp, "/", cmd);
		if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
			break ;
		else if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == -1)
			syntax_error("%s: Permission denied.", cmd);
		ft_strdel(&full_path);
		tmp++;
	}
	if (full_path == NULL)
		syntax_error("%s: Command not found", cmd);
	ft_free_strtab(all_paths);
	return (full_path);
}

int				exec_builin(t_list *env, t_list *blt, t_node *node)
{
	char		**cmds;

	cmds = NULL;
	if (blt && (*(cmds = node_to_char(node->spec.simple_command)) != NULL))
	{
		((t_builtin*)blt->content)->f(cmds + 1, &env);
		ft_free_strtab(cmds);
	}
	return (0);
}

int		assert_tok(t_token_kind base, t_token_kind k2, t_token_kind k3)
{
	if (base == k2 || base == k3)
		return (1);
	return (0);
}

int		execute_redirection(t_redirection *list)
{
	t_redirection	*current;

	DECLARE(int, _(error, 0));
	current = list;
	while (current && !error)
	{
		if (current->kind == '>' || current->kind == TOKEN_CLOBBER)
			error = output(current);
		else if (current->kind == TOKEN_DGREAT)
			error = output_append(current);
		else if (assert_tok(current->kind, TOKEN_GREATAND, TOKEN_ANDGREAT))
			error = output_with_aggregate(current, 0);
		else if (assert_tok(current->kind, TOKEN_DGREATAND, TOKEN_AND_DEGREATE))
			error = output_with_aggregate(current, 1);
		else if (current->kind == '<')
			error = input(current);
		else if (current->kind == TOKEN_DLESS)
			error = input_here_doc(current);
		else if (current->kind == TOKEN_LESSAND)
			error = input_with_aggregate(current);
		else if (current->kind == TOKEN_LESSGREAT)
			error = input_output(current);
		current = current->next;
	}
	return (error);
}
