#include "shell.h"

int		check_file_status(char *filename)
{
	int error;

	error = 0;

	if (access(filename, F_OK) == -1)
			error = ft_printf_fd(2, "42sh: no such file or directory: %s\n", filename);
	else if (access(filename, R_OK) == -1)
			error = ft_printf_fd(2, "42sh: permission denied: %s\n", filename);
	return (error);
}

void	output(t_redirection *redir)
{
	if ((redir->fd2 = open(redir->word, O_RDWR | O_CREAT | O_TRUNC, 0644)))
		dup2(redir->fd2, redir->fd1);
}

void	output_append(t_redirection *redir)
{
	if ((redir->fd2 = open(redir->word, O_RDWR | O_CREAT | O_APPEND, 0644)))
		dup2(redir->fd2, redir->fd1);
}

void	output_with_aggregate(t_redirection *redir)
{
	if (redir->fd2 < 0)
	{
		if (redir->word && redir->word[0] == '-')
		{
			if (redir->fd2 == -1)
				close(redir->fd1);
			return ;
		}
		else
			redir->fd2 = open(redir->word, O_RDWR | O_CREAT | O_TRUNC, 0644);
	} else
	{
		if (redir->word && redir->word[0] == '-')
		{
			if (redir->fd2 >= 0)
			{
				dup2(redir->fd1, redir->fd2);
				close(redir->fd2);
				return ;
			}
		}
	}
	if (redir->fd2 >= 0)
	{
		if (redir->fd1 == 1)
			dup2(redir->fd2, 2);
		dup2(redir->fd2, redir->fd1);
	}
}

void	output_with_aggregate_append(t_redirection *redir)
{
	ft_printf_fd(2, "To be handled later on\n");
}

/*
** Input redirection part
*/

void	input(t_redirection *redir)
{
	if (check_file_status(redir->word) == 0)
	{
		if ((redir->fd2 = open(redir->word, O_RDONLY)))
			dup2(redir->fd2, redir->fd1);
	}
}

void	input_here_doc(t_redirection *redir)
{
	char	*path;
	int		fd;

	path = ft_strjoin("/tmp/", redir->herdoc);
	if (check_file_status(path) == 0)
	{
		if ((fd = open(path, O_RDONLY)))
			dup2(fd, redir->fd1);
	}
	ft_strdel(&path);
}
