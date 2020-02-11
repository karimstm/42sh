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

int		execute_redirection(t_redirection *list)
{
	t_redirection	*current;
	int				error;

	error = 0;
	current = list;
	while (current)
	{
		if (error)
			return (1);
		if (current->kind == '>' || current->kind == TOKEN_CLOBBER)
			error = output(current);
		else if (current->kind == TOKEN_DGREAT)
			error = output_append(current);
		else if (current->kind == TOKEN_GREATAND || current->kind == TOKEN_ANDGREAT)
			error = output_with_aggregate(current, 0);
		else if (current->kind == TOKEN_DGREATAND || current->kind == TOKEN_AND_DEGREATE)
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
