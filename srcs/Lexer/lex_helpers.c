/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:43:44 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 16:44:19 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int				match_token(t_token_kind kind)
{
	if (is_token(kind))
	{
		next_token();
		return (1);
	}
	else
		return (0);
}

int				match_token_test(t_token_kind kind)
{
	while (is_token(TOKEN_SPACE))
		next_token();
	if (is_token(kind))
	{
		next_token();
		return (1);
	}
	else
		return (0);
}

void			init_stream(const char *str)
{
	char	*new_str;

	new_str = ft_strdup(str);
	g_line = new_str;
	g_token.line = new_str;
	g_token.current = ft_strdup(new_str);
	escape_space();
}
