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
	if (current == NULL)
		return (NULL);
	cmds = (char **)malloc(sizeof(char *) * (command->node_count + 1));
	i = 0;
	while (current)
	{
		cmds[i++] = ft_strdup(current->name);
		current = current->next;
	}
	cmds[i] = NULL;
	return (cmds);
}

char			*working_path(t_list *env, char *cmd)
{
	char	**all_paths;
	char	**tmp;
	char	*full_path;

	all_paths = get_path(env);
	tmp = all_paths;
	full_path = NULL;
	while (*tmp)
	{
		full_path = ft_strjoin_pre(*tmp, "/", cmd);
		if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
			break ;
		else if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == -1)
			syntax_error("%s: Permission denied.", cmd); // needs more check later on
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
	if(blt && (*(cmds = node_to_char(node->spec.simple_command)) != NULL))
	{
		((t_builtin*)blt->content)->f(cmds + 1, &env);
		ft_free_strtab(cmds);
	}
	return (0);
}


t_node			*pipe_to_stack(t_node *node, t_stack *stack)
{
	if (node)
	{
		if (node->kind == NODE_PIPE)
		{
			if (node->spec.and_or_command->left)
				pipe_to_stack(node->spec.and_or_command->left, stack);
			if (node->spec.and_or_command->right)
				pipe_to_stack(node->spec.and_or_command->right, stack);
		} else
			push_to_stack(stack, node);
	}
	return (node);
}


void			execute_redirection(t_redirection *list)
{
	t_redirection	*current;

	current = list;
	while (current)
	{
		if (current->kind == '>' || current->kind == TOKEN_CLOBBER)
			output(current);
		else if (current->kind == TOKEN_DGREAT)
			output_append(current);
		else if (current->kind == TOKEN_GREATAND)
			output_with_aggregate(current);
		else if (current->kind == TOKEN_DGREATAND)
			output_with_aggregate_append(current);
		else if (current->kind == '<')
			input(current);
		else if (current->kind == TOKEN_DLESS)
			input_here_doc(current);
		else if (current->kind == TOKEN_LESSAND)
			input_with_aggregate(current);
		else if (current->kind == TOKEN_LESSGREAT)
			input_output(current);
		current = current->next;
	}
}
