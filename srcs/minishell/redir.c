#include "shell.h"

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

void	output(t_redirection *redir)
{
	if (!check_file_status(redir->word))
	{
		if ((redir->fd2 = open(redir->word, O_WRONLY | O_CREAT | O_TRUNC, 0644)))
			dup2(redir->fd2, redir->fd1);
		if (redir->fd2 != redir->fd1)
			close(redir->fd2);
	}
}

void	output_append(t_redirection *redir)
{
	if (!check_file_status(redir->word))
	{
		if ((redir->fd2 = open(redir->word, O_WRONLY | O_CREAT | O_APPEND, 0644)))
			dup2(redir->fd2, redir->fd1);
		if (redir->fd2 != redir->fd1)
			close(redir->fd2);
	}
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
		else if (!check_file_status(redir->word))
			redir->fd2 = open(redir->word, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
	syntax_error("To be handled later on");
}

/*
** Input redirection part
*/

void	input(t_redirection *redir)
{
	if (check_file_status(redir->word) == 0)
	{
		if ((redir->fd2 = open(redir->word, O_RDONLY)))
		{
			dup2(redir->fd2, redir->fd1);
			if (redir->fd2 != redir->fd1)
				close(redir->fd2);
		}
	}
}

void	input_here_doc(t_redirection *redir)
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
		}
	}
	ft_strdel(&path);
}


void	input_with_aggregate(t_redirection *redir)
{
	if (redir->fd2 < 0)
	{
		if (redir->word && redir->word[0] == '-')
		{
			if (redir->fd2 == -1)
				close(redir->fd1);
			return ;
		}
	}
	else
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


void		input_output(t_redirection *redir)
{
	if (check_file_status(redir->word) == 0)
	{
		if ((redir->fd2 = open(redir->word, O_RDWR | O_CREAT, 0644)))
		{
			dup2(redir->fd2, redir->fd1);
			if (redir->fd2 != redir->fd1)
				close(redir->fd2);
		}
	}
}
