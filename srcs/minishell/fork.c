/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/05 22:36:27 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/05/09 00:39:28 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	handle_errors(char status, char exit_flag)
{
	if (status == -1)
		status = PERM_DENIED;
	else if (status == -2)
		status = SYNTAX_ERROR;
	if (status == PERM_DENIED)
		ft_printf_fd(2, "21sh: Permission denied\n");
	else if (status == PERM_DENIED2)
		ft_printf_fd(2, "21sh: Permission denied or file not found\n");
	else if (status == BAD_DESCRIPTOR)
		ft_printf_fd(2, "21sh: Bad file descriptor\n");
	else if (status == SYNTAX_ERROR)
		ft_printf_fd(2, "21sh: Ambiguous redirect\n");
	if (exit_flag)
		exit(EXIT_FAILURE);
}

/*
**  The Fork Function
**  it creates a copy (child process) of the current process (parent process)
**  man fork -> for more info.
**  after that there will be two processes one is parent and the other is child
**  the child executes the cmds with the environment given on the parameters
**  the parent waits the child to finish
*/

void	forkit(char *path, t_list **env, t_token *token, int pipe[2])
{
	pid_t		child;
	char		**cmds;
	char		**env_tab;
	int			status;

	env_tab = env_to_tab(*env);
	signal(SIGINT, child_handler);
	child = fork();
	if (child > 0 && check_pipe(token) == 0)
		waitpid(child, &status, 0);
	else if (child == 0)
	{
		dup2(pipe[0], 0);
		dup2(pipe[1], 1);
		if ((status = handle_redirection(token, NULL)) != 0)
		{
			ft_free_strtab(env_tab);
			handle_errors(status, 1);
		}
		if (*(cmds = list_to_chars(token)) == NULL)
			return ;
		execve(path, cmds, env_tab);
	}
	ft_free_strtab(env_tab);
	signals();
}
