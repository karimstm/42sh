/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   left_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 02:45:45 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/05/09 00:37:02 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void		heredoc(t_token *token, int fd_to_rep)
{
	int		pp[2];

	if (token->token != NULL)
	{
		pipe(pp);
		dup2(pp[0], fd_to_rep);
		ft_printf_fd(pp[1], "%s\n", token->token);
		close(pp[0]);
		close(pp[1]);
	}
}

static char		theredoc(t_token *token, int fd_to_rep)
{
	int		fd;
	int		pp[2];
	char	buf;

	if ((fd = open(token->token, O_RDONLY)) == -1)
		return (PERM_DENIED2);
	pipe(pp);
	dup2(pp[0], fd_to_rep);
	while (read(fd, &buf, 1) > 0)
		write(pp[1], &buf, 1);
	close(pp[0]);
	close(pp[1]);
	close(fd);
	return (0);
}

static t_token	*get_left_redirect(t_token *token)
{
	t_token		*left;
	int			tok_type;

	left = NULL;
	while (token && !(token->tok_type & SH_SEMI) &&
			!(token->tok_type & SH_PIPE))
	{
		if ((token->tok_type & SH_REDIRECTION) && ft_strchr(token->token, '<'))
		{
			left = token;
			tok_type = left->tok_type;
			token->tok_type = 0;
			if (token->next)
				token->next->tok_type = 0;
		}
		token = token->next;
	}
	if (left)
		left->tok_type = tok_type;
	return (left);
}

static char		handle_left_redirect(t_token *token)
{
	t_token		*left;
	char		*file;
	int			fd_to_rep;

	left = get_left_redirect(token);
	if (left != NULL)
	{
		fd_to_rep = 0;
		if ((left->tok_type & SH_WORD) != 0)
			fd_to_rep = ft_atoi(left->token);
		left->tok_type = 0;
		file = ft_strchr(left->token, '<') + 1;
		if (*file == '\0' && left->next)
			return (theredoc(left->next, fd_to_rep));
		else if (*file == '<' && left->next)
			heredoc(left->next, fd_to_rep);
		else
			return (SYNTAX_ERROR);
	}
	return (0);
}

char			handle_redirection(t_token *token, int *fd_backup)
{
	char	status;

	if ((status = handle_left_redirect(token)) != 0)
		return (status);
	if ((status = handle_right_redirect(token, fd_backup)) != 0)
		return (status);
	return (0);
}
