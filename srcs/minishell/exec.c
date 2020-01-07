#include "shell.h"

void			restore_fds(int std[2])
{
	dup2(std[0], 0);
	dup2(std[1], 1);
}

char        **node_to_char(t_list_simple_command *command)
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
		cmds[i++] = current->name;
		current = current->next;
	}
	cmds[i] = NULL;
	return (cmds);
}


char		*working_path(t_list *env, char *cmd)
{
	char	**all_paths;
	char	**tmp;
	char	*full_path;

	all_paths = get_path(env);
	tmp = all_paths;
	while (*tmp)
	{
		full_path = ft_strjoin_pre(*tmp, "/", cmd);
		if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
			return (full_path);
		else if (access(full_path, F_OK) == 0)
			ft_printf_fd(1, "%s: Permission denied.\n"); // needs more check later on
		tmp++;
	}
	return (NULL);
}

int			my_fork(char *path, t_list *env, char **cmds, int std[2])
{
	pid_t		child;
	char		**env_tab;
	int			status;

	status = 0;
	env_tab = env_to_tab(env);
	child = fork();
	if (child > 0)
		waitpid(child, &status, 0);
	else if (child == 0)
		execve(path, cmds, env_tab);
	return (status);
}


int        execute_shell(t_list *blt, t_list *env, t_node *node, int std[2])
{
	t_simple_command *current;
	char			*path;
	char			**cmds;

	(void)blt;
	current = (node && node->simple_command) ? node->simple_command->head : NULL;
	if (current == NULL)
		return(0) ;
	if (ft_strcmp(current->name, "exit") == 0)
	{
		ft_printf_fd(2, "exiting...\n");
		exit(0);
	}
	if ((path = working_path(env, current->name)) == NULL)
		return(127);
	cmds = node_to_char(node->simple_command);
	return (my_fork(path, env, cmds, std));
}


t_node		*pipe_to_stack(t_node *node, t_stack *stack)
{
	if (node)
	{
		if (node->kind == NODE_PIPE)
		{
			if (node->and_or_command->left)
				pipe_to_stack(node->and_or_command->left, stack);
			if (node->and_or_command->right)
				pipe_to_stack(node->and_or_command->right, stack);
		} else
			push_to_stack(stack, node);
	}
	return (node);
}

void		set_fds(int tmp_stds[3])
{
	tmp_stds[0] = dup(0);
	tmp_stds[1] = dup(1);
	tmp_stds[2] = dup(2);
}

int			pipe_stack_execute(t_stack *stack, t_list *blt, t_line *line, int std[2])
{
	int		status;
	int		pp[2];
	t_node 	*poped;
	int		tmp[3];

	std[0] = 0;
	std[1] = 1;
	status = 0;
	reverse_stack(stack);
	set_fds(tmp);
	poped = pop_stack(stack);
	while (!is_underflow(stack))
	{
		pipe(pp);
		dup2(pp[1], 1);
		status = execute_cmd(poped, blt, line, std);
		close(pp[1]);
		dup2(pp[0], 0);
		poped = pop_stack(stack);
	}
	dup2(tmp[1], 1);
	if (poped)
		status = execute_cmd(poped, blt, line, std);
	restore_std(tmp);
	return (status);
}

int			handle_sep(t_node *node, t_list *blt, t_line *line, int std[2])
{
	int status;

	status = 0;
	if (node->sep_op_command->left)
		status = execute_cmd(node->sep_op_command->left, blt, line, std);
	if (node->sep_op_command->right)
        status = execute_cmd(node->sep_op_command->right, blt, line, std);
	return (status);
}

int			handle_condition(t_node *node, t_list *blt, t_line *line, int std[2])
{
	int status;

	status = 0;
	if (node->and_or_command->left)
		status = execute_cmd(node->and_or_command->left, blt, line, std);
	if (node->and_or_command->right
		&& ((status == 0 && node->and_or_command->kind == TOKEN_AND_IF)
		|| (status && node->and_or_command->kind == TOKEN_OR_IF)))
		status = execute_cmd(node->and_or_command->right, blt, line, std);
	return (status);
}

void	execute_redirection(t_redirection *list)
{
	t_redirection	*current;

	current = list;
	while (current)
	{
		if (current->kind == '>')
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
		current = current->next;
	}
}

int			execute_cmd(t_node *node, t_list *blt, t_line *line, int std[2])
{
	int status;
	int	tmp_stds[3];

	status = 0;
	if (node)
	{
		if (node->redir)
		{
			set_fds(tmp_stds);
			execute_redirection(reverse_redirection(node->redir));

		}
		if (node->kind == NODE_SEMI_AND)
			status = handle_condition(node, blt, line, std);
		else if (node->kind == NODE_AND_OR)
			status = handle_condition(node, blt, line, std);
		else if (node->kind == NODE_PIPE)
		{
			t_stack stack;

			init_stack(&stack, 100);
			node = pipe_to_stack(node, &stack);
			status = pipe_stack_execute(&stack, blt, line, std);
			free(stack.lists);
		}
		else if (node->kind == NODE_SIMPLE_COMMAND)
			status = execute_shell(blt, line->env, node, std);
		if (node->redir)
			restore_std(tmp_stds);
	}
	return (status);
}
