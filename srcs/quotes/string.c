/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 16:12:36 by amoutik           #+#    #+#             */
/*   Updated: 2019/05/07 09:48:45 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"

void				new_string(t_string *str)
{
	str->cap = 1;
	str->len = 0;
	if (str->string)
		ft_strdel(&str->string);
	str->string = ft_strnew(str->cap);
}

void				push(t_string *str, char c)
{
	if (str->cap > str->len + 1)
	{
		str->string[str->len++] = c;
		str->string[str->len] = '\0';
	}
	else
	{
		while (str->cap <= str->len + 1)
			str->cap *= 2;
		str->string = (char *)xrealloc(str->string, str->cap);
		str->string[str->len++] = c;
		str->string[str->len] = '\0';
	}
}

char				space(char c)
{
	if (c == 'n')
		return ('\n');
	else if (c == 'a')
		return ('\a');
	else if (c == 'f')
		return ('\f');
	else if (c == 'v')
		return ('\v');
	else if (c == 'r')
		return ('\r');
	else if (c == 'b')
		return ('\b');
	else
		return (0);
}

int					insert_token(t_token_list *list,
		t_string *str, enum e_token_type type)
{
	if (str->len)
	{
		push_token(list, ft_strdup(str->string), type);
		new_string(str);
	}
	return (1);
}
