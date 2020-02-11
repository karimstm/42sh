#include "shell.h"

int		fd_is_valid(int fd)
{
    return (fcntl(fd, F_GETFD) != -1);
}

int		bad_fd(int fd)
{
	syntax_error("42sh: %d: Bad file descriptor", fd);
	return (1);
}

int		check_file_permission(char *filename, int perm)
{
	if (access(filename, F_OK) == 0 && access(filename, perm) == -1)
	{
		syntax_error("42sh: permission denied: %s", filename);
		return (0);
	}
	return (1);
}

int		check_file_status(char *filename)
{
	int error;

	error = 1;
	if (is_directory(filename))
		syntax_error("42sh: %s: Is a Directory.", filename);
	else if (access(filename, F_OK) == -1)
		syntax_error("42sh: no such file or directory: %s", filename);
	else if (access(filename, R_OK) == -1)
		syntax_error("42sh: permission denied: %s", filename);
	else
		error = 0;
	return (error);
}

int		output(t_redirection *redir)
{
	if (check_file_permission(redir->word, W_OK))
	{
		if ((redir->fd2 = open(redir->word, O_WRONLY | O_CREAT | O_TRUNC, 0644)))
			dup2(redir->fd2, redir->fd1);
		if (redir->fd2 != redir->fd1)
			close(redir->fd2);
		return (0);
	}
	return (1);
}

int		output_append(t_redirection *redir)
{
	if (check_file_permission(redir->word, W_OK))
	{
		if ((redir->fd2 = open(redir->word, O_WRONLY | O_CREAT | O_APPEND, 0644)))
			dup2(redir->fd2, redir->fd1);
		if (redir->fd2 != redir->fd1)
			close(redir->fd2);
		return (0);
	}
	return (1);
}

int		output_with_aggregate(t_redirection *redir, int is_append)
{
	if (redir->fd2 < 0)
	{
		if (redir->word && redir->word[0] == '-')
		{
			if (redir->fd2 == -1)
				close(redir->fd1);
			return (0);
		}
		else if (check_file_permission(redir->word, W_OK))
		{
			if (!is_append)
				redir->fd2 = open(redir->word, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				redir->fd2 = open(redir->word, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		else
			return (1);
	} else
	{
		if (redir->word && redir->word[0] == '-')
		{
			if (fd_is_valid(redir->fd2))
			{
				dup2(redir->fd1, redir->fd2);
				close(redir->fd2);
				return (0);
			}
			else
				return (bad_fd(redir->fd2));
		}
	}
	if (fd_is_valid(redir->fd2))
	{
		if (redir->fd1 == 1)
			dup2(redir->fd2, 2);
		dup2(redir->fd2, redir->fd1);
		return (0);
	}
	else
		return (bad_fd(redir->fd2));
}

int		output_with_aggregate_append(t_redirection *redir)
{
	(void)redir;
	syntax_error("To be handled later on");
	return (0);
}

/*
** Input redirection part
*/

int		input(t_redirection *redir)
{
	if (check_file_status(redir->word) == 0)
	{
		if ((redir->fd2 = open(redir->word, O_RDONLY)))
		{
			dup2(redir->fd2, redir->fd1);
			if (redir->fd2 != redir->fd1)
				close(redir->fd2);
			return (0);
		}
	}
	return (1);
}

int		input_here_doc(t_redirection *redir)
{
	char	*path;

	path = ft_strjoin("/tmp/", redir->herdoc);
	if (check_file_status(path) == 0)
	{
		if ((redir->fd2 = open(path, O_RDONLY)))
		{
			dup2(redir->fd2, redir->fd1);
			if (redir->fd2 != redir->fd1)
				close(redir->fd2);
			return (0);
		}
	}
	ft_strdel(&path);
	return (1);
}


int		input_with_aggregate(t_redirection *redir)
{
	if (redir->fd2 < 0)
	{
		if (redir->word && redir->word[0] == '-')
		{
			if (redir->fd2 == -1)
				close(redir->fd1);
			return (0);
		}
	}
	else
	{
		if (redir->word && redir->word[0] == '-')
		{
			if (fd_is_valid(redir->fd2))
			{
				dup2(redir->fd1, redir->fd2);
				close(redir->fd2);
				return (0);
			}
			else
				return (bad_fd(redir->fd2));
		}
	}
	if (fd_is_valid(redir->fd2))
	{
		if (redir->fd1 == 1)
			dup2(redir->fd2, 2);
		dup2(redir->fd2, redir->fd1);
		return (0);
	}
	else
		return (bad_fd(redir->fd2));
}


int		input_output(t_redirection *redir)
{
	if (check_file_status(redir->word) == 0)
	{
		if ((redir->fd2 = open(redir->word, O_RDWR | O_CREAT, 0644)))
		{
			dup2(redir->fd2, redir->fd1);
			if (redir->fd2 != redir->fd1)
				close(redir->fd2);
			return (0);
		}
	}
	return (1);
}
