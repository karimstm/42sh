/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 14:32:37 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/05/09 02:31:54 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char		check_pipe(t_token *token)
{
	while (token)
	{
		if (token->tok_type & SH_PIPE)
			return (1);
		if (token->tok_type & SH_SEMI)
			return (0);
		token = token->next;
	}
	return (0);
}

void		next_pipe(t_token_list *tokens)
{
	t_token		*token;

	token = tokens->head;
	while (token)
	{
		if ((token->tok_type & SH_PIPE) || (token->tok_type & SH_SEMI))
		{
			tokens->head = token->next;
			return ;
		}
		token = token->next;
	}
}

t_token		*get_cmd_token(t_token *ptr)
{
	int			count;

	count = 0;
	while (ptr && !(ptr->tok_type & SH_SEMI) && !(ptr->tok_type & SH_PIPE))
	{
		if (ptr->tok_type & SH_REDIRECTION)
			count = 1;
		else if (count == 1 && (ptr->tok_type & SH_WORD))
			count = 0;
		else if (count == 0 && (ptr->tok_type & SH_WORD))
			return (ptr);
		ptr = ptr->next;
	}
	return (NULL);
}

static int	count_nodes(t_token *node)
{
	int		count;

	count = 0;
	while (node && !(node->tok_type & SH_SEMI) && !(node->tok_type & SH_PIPE))
	{
		count++;
		node = node->next;
	}
	return (count);
}

char		**list_to_chars(t_token *token)
{
	char		**cmds;
	int			count;
	t_token		*node;

	node = get_cmd_token(token);
	cmds = (char**)ft_memalloc(sizeof(char*) * (count_nodes(node) + 1));
	if (cmds == NULL)
		return (NULL);
	count = 0;
	while (node)
	{
		if ((node->tok_type & SH_SEMI) || (node->tok_type & SH_PIPE))
			break ;
		if (node->tok_type & SH_WORD)
			cmds[count++] = ft_strdup(node->token);
		node = node->next;
	}
	cmds[count] = NULL;
	return (cmds);
}
