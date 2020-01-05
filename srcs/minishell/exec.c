#include "shell.h"


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
	{
		dup2(std[0], 0);
		dup2(std[1], 1);
		execve(path, cmds, env_tab);
	}
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
		exit(0);
	if ((path = working_path(env, current->name)) == NULL)
		return(127);
	cmds = node_to_char(node->simple_command);
	return (my_fork(path, env, cmds, std));
}

// int			and_or_pipe_exec(t_node *node, t_list *blt, t_line *line)
// {
// 	return (0);
// }

int 		check_next_pipe(t_node *node)
{
	if (node->kind == NODE_PIPE)
		return (1);
	return (0);
}

int			execute_cmd(t_node *node, t_list *blt, t_line *line, int std[2])
{
	int status;
	int pp[2];
	int flag;

	status = 0;
	flag = 0;
	if (node)
	{
		if (node->kind == NODE_SEMI_AND)
		{
			if (node->sep_op_command->left)
				execute_cmd(node->sep_op_command->left, blt, line, std);
			if (node->sep_op_command->right)
                execute_cmd(node->sep_op_command->right, blt, line, std);
		}
		else if (node->kind == NODE_AND_OR)
		{
			if (node->and_or_command->left)
                status = execute_cmd(node->and_or_command->left, blt, line, std);
            if (node->and_or_command->right
				&& ((status == 0 && node->and_or_command->kind == TOKEN_AND_IF)
				|| (status && node->and_or_command->kind == TOKEN_OR_IF)))
                status = execute_cmd(node->and_or_command->right, blt, line, std);
		}
		else if (node->kind == NODE_PIPE)
		{
			printf("==== %d %d\n", std[0], std[1]);
			if (node->and_or_command->left)
			{
				pipe(pp);
				std[1] = pp[1];
				status = execute_cmd(node->and_or_command->left, blt, line, std);
				close(std[1]);
			}
			if (node->and_or_command->right)
			{
				std[0] = pp[0];
				std[1] = 1;
				if (check_next_pipe(node->and_or_command->left) && pipe(pp) != -1)
					std[1] = pp[1];
				status = execute_cmd(node->and_or_command->right, blt, line, std);
				close(std[0]);
			}
		}
		else if (node->kind == NODE_SIMPLE_COMMAND)
		{
			dprintf(2, "====%s=== and stdin %d stdout %d\n", node->simple_command->head->name, std[0], std[1]);
			status = execute_shell(blt, line->env, node, std);
		}
	}
	return (status);
}
