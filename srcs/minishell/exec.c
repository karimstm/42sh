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

int				my_fork(char *path, t_list *env, char **cmds, t_list_process *process, t_job_kind kind)
{
	pid_t		child;
	char		**env_tab;
	int			status;
	t_process	*current;

	status = 0;
	current = (process && process->head) ? process->head : NULL;
	while (current)
	{
		current->argv = node_to_char(current->node->simple_command);
		ft_printf_fd(2, "%s\n", path);
		if (path != NULL)
		{
			ft_strdel(&current->argv[0]);
			current->argv[0] = ft_strdup(path);
		}
		env_tab = env_to_tab(env);
		child = fork();
		if (child > 0)
		{
			current->pid = child;
			if(shell_is_interactive)
			{
				if (!jobs.head->pgid)
					jobs.head->pgid = child;
				setpgid(child, jobs.tail->pgid);
			}
			tcsetpgrp(STDIN_FILENO, shell_pgid);
		}
		else if (child == 0)
		{
			if(shell_is_interactive)
			{
				if (!jobs.head->pgid)
					jobs.head->pgid = child;
				setpgid(getpid(), jobs.tail->pgid);
			}
			launch_process(current, getpgrp(), 0, 1, 2, kind == J_FOREGROUND ? 1 : 0, env_tab);
		}
		if (!shell_is_interactive)
			wait_for_job(jobs.head);
		else if (kind == J_FOREGROUND)
		{
			put_job_in_foreground(jobs.head, 0);
		} else if (kind == J_BACKGROUND)
		{
			put_job_in_background(jobs.head, 0);
		}
		current = current->next;
	}
	return (status);
}

int				exec_builin(t_list *env, t_list *blt, t_node *node)
{
	char		**cmds;

	cmds = NULL;
	if(blt && (*(cmds = node_to_char(node->simple_command)) != NULL))
	{
		((t_builtin*)blt->content)->f(cmds + 1, &env);
		ft_free_strtab(cmds);
	}
	return (0);
}

int				exec_localy(t_list *env, t_list_simple_command *node, t_list_process *process, t_job_kind kind)
{
	char	**cmds;
	int		status;

	status = 0;
	cmds = NULL;
	ft_printf_fd(2, "=== %s ===\n", node->head->name);
	if (check_file_status(node->head->name) == 0)
	{
		cmds = node_to_char(node);
		status = my_fork(cmds[0], env, cmds, process, kind);
		ft_free_strtab(cmds);
	}
	return (status);
}

int				execute_shell(t_list *blt, t_list *env, t_node *node, t_list_process *process, t_job_kind kind)
{
	t_simple_command	*current;
	char				*path;
	char				**cmds;
	t_list				*bltin;
	int					status;
	current = (node && node->simple_command) ? node->simple_command->head : NULL;
	if (current == NULL)
		return(0) ;
	if (ft_strcmp(current->name, "exit") == 0)
	{
		ft_printf_fd(2, "exiting...\n");
		exit(0);
	}
	if (ft_strchr(current->name, '/'))
		return exec_localy(env, node->simple_command, process, kind);
	if ((bltin = ft_lstsearch(blt, current->name, &check_builtin)) != NULL)
		return exec_builin(env, bltin, node);
	else if ((path = working_path(env, current->name)) == NULL)
		return(127);
	else
	{
		cmds = node_to_char(node->simple_command);
		status = my_fork(path, env, cmds, process, kind);
		ft_strdel(&path);
		ft_free_strtab(cmds);
		return (status);
	}
}


t_node			*pipe_to_stack(t_node *node, t_stack *stack)
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

void			set_fds(int tmp_stds[3])
{
	tmp_stds[0] = dup(0);
	tmp_stds[1] = dup(1);
	tmp_stds[2] = dup(2);
}

int				pipe_stack_execute(t_stack *stack, t_list *blt, t_line *line, int fd_out)
{
	int		status;
	int		pp[2];
	t_node 	*poped;

	ft_printf_fd(2, "Parent Group %lld\n", getpgrp());
	status = 0;
	reverse_stack(stack);
	poped = pop_stack(stack);
	while (!is_underflow(stack))
	{
		pipe(pp);
		dup2(pp[1], 1);
		close(pp[1]);
		status = execute_cmd(poped, blt, line, J_FOREGROUND, NULL);
		dup2(pp[0], 0);
		poped = pop_stack(stack);
		close(pp[0]);
	}
	dup2(fd_out, 1);
	if (poped)
		status = execute_cmd(poped, blt, line, J_FOREGROUND, NULL);
	close(pp[0]);
	close(pp[1]);
	return (status);
}


int				handle_sep(t_node *node, t_list *blt, t_line *line)
{
	int status;
	t_list_process *process;

	status = 0;
	process = NULL;
	if (node->sep_op_command->left)
	{
		if (node->sep_op_command->kind == '&')
		{
			process = (t_list_process *)xmalloc(sizeof(t_list_process));
			init_process_list(process);
			process_push(process, 0, NULL, node->sep_op_command->left);
			job_push(&jobs, process, 0);
			status = execute_cmd(node->sep_op_command->left, blt, line, J_BACKGROUND, process);
		}
		else
			status = execute_cmd(node->sep_op_command->left, blt, line, J_FOREGROUND, process);
	}
	if (node->sep_op_command->right)
        status = execute_cmd(node->sep_op_command->right, blt, line, J_FOREGROUND, process);
	return (status);
}

int				handle_condition(t_node *node, t_list *blt, t_line *line)
{
	int status;

	status = 0;
	if (node->and_or_command->left)
		status = execute_cmd(node->and_or_command->left, blt, line, J_FOREGROUND, NULL);
	if (node->and_or_command->right
		&& ((status == 0 && node->and_or_command->kind == TOKEN_AND_IF)
		|| (status && node->and_or_command->kind == TOKEN_OR_IF)))
		status = execute_cmd(node->and_or_command->right, blt, line, J_FOREGROUND, NULL);
	return (status);
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

int				initial_pipe(t_node *node, t_list *blt, t_line *line)
{
	t_stack stack;
	int		status;
	int		tmp[3];

	set_fds(tmp);
	status = 0;
	init_stack(&stack, 100);
	node = pipe_to_stack(node, &stack);
	status = pipe_stack_execute(&stack, blt, line, tmp[1]);
	restore_std(tmp);
	free(stack.lists);
	return (status);
}

int				execute_cmd(t_node *node, t_list *blt, t_line *line, t_job_kind kind, t_list_process *process)
{
	int				status;
	t_list_process	*tmp;
	int				tmp_stds[3];

	status = 0;
	if (node)
	{
		if (node->redir)
		{
			set_fds(tmp_stds);
			execute_redirection((node->redir = reverse_redirection(node->redir)));
			if ((status = *error_num()))
			{
				restore_std(tmp_stds);
				reset_error_num();
				return (status);
			}
		}
		if (node->kind == NODE_SEMI_AND)
			status = handle_sep(node, blt, line);
		else if (node->kind == NODE_AND_OR)
			status = handle_condition(node, blt, line);
		else if (node->kind == NODE_PIPE)
			initial_pipe(node, blt, line);
		else if (node->kind == NODE_SIMPLE_COMMAND)
		{
			if (process == NULL)
			{
				tmp = (t_list_process *)xmalloc(sizeof(t_list_process));
				init_process_list(tmp);
				process_push(tmp, 0, NULL, node);
				job_push(&jobs, tmp, 0);
				process = tmp;
			}
			status = execute_shell(blt, line->env, node, process, kind);
		}
		if (node->redir)
		{
			free_redir(&node->redir);
			restore_std(tmp_stds);
		}
	}
	return (status);
}
