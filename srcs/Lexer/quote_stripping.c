/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_stripping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:25:04 by amoutik           #+#    #+#             */
/*   Updated: 2020/03/01 12:57:33 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
**	karim \<new_line>karim
**	\
*/

void					skip_dqoute_w(char **string, char c,
						t_string *str, int include)
{
	char	*new;

	new = *string;
	include ? push(str, *new++) : new++;
	while (new && *new)
	{
		if (!include && *new == '\\' && c == '"' && *(new + 1) == '"')
		{
			new++;
			push(str, *new++);
			continue ;
		}
		if (*new == '\\' && c != '\'')
		{
			if (back_slash_escape(&new, str, include))
				continue ;
		}
		else if (*new == c)
		{
			include ? push(str, *new++) : new++;
			break ;
		}
		*new ? push(str, *new++) : 0;
	}
	*string = new;
}

/*
**	something"To Test with" like this one 'here ' and\<new_line>now
**	=> something"To Test with"
**	=> like
**	=> this
**	=> one
**	=> 'here '
**	=> andnow
*/

void					word_looping(t_list_simple_command *list,
						t_string *str, char **word, t_token_kind kind)
{
	char	*s;

	s = *word;
	while (*s != EOS)
	{
		if (ft_isspace(*s) || *s == EOS)
			break ;
		if (*s == '\'' || *s == '"')
			skip_dqoute_w(&s, *s, str, 1);
		else
		{
			if (*s == '\\')
				back_slash_escape(&s, str, 1);
			if (*s)
				push(str, *s++);
		}
	}
	token_push(list, str->string, kind);
	*word = s;
}

t_list_simple_command	*split_word(char *word, t_token_kind kind)
{
	t_string				string;
	char					*start;
	t_list_simple_command	*list;

	list = malloc_list_simple_command();
	start = word;
	string.string = NULL;
	new_string(&string);
	if (word == NULL)
	{
		token_push(list, string.string, kind);
		return (list);
	}
	while (*word)
	{
		while (ft_isspace(*word))
			word++;
		if (*word)
			word_looping(list, &string, &word, kind);
		string.string = NULL;
		if (*word)
			new_string(&string);
	}
	if (!list->node_count)
		ft_strdel(&string.string);
	return (list);
}

/*
**	NODE -> NODE -> NODE
**		new -> new -> new
**		TMP -> TMP -> TMP
**	new -> new -> new -> TMP -> TMP -> TMP -> NODE
**					|
**					|
**					-->next = TMP
*/

void					init_expansion(t_list_simple_command *list)
{
	t_list_simple_command	*res;

	DECLARE(t_simple_command, *cur, *next, _(*prev, NULL));
	cur = list && list->head ? list->head : NULL;
	while (cur)
	{
		next = cur->next;
		if (cur->name && (res = split_word(cur->name, cur->kind))->node_count)
		{
			if (prev)
				prev->next = res->head;
			else
				list->head = res->head;
			prev = res->tail;
			list->node_count += res->node_count;
		}
		else if (cur == list->head)
			list->head = NULL;
		free(res);
		ft_strdel(&cur->name);
		list->node_count--;
		free(cur);
		cur = next;
	}
	prev ? list->tail = prev : 0;
}

char					*quote_stripping(char *str)
{
	t_string	string;
	char		*tmp;
	char		c;

	string.string = NULL;
	new_string(&string);
	tmp = str;
	c = 0;
	while (tmp && *tmp)
	{
		if (*tmp == '\\')
		{
			tmp++;
			if (*tmp == '\n')
				tmp++;
		}
		else if (*tmp == '"' || *tmp == '\'')
			skip_dqoute_w(&tmp, *tmp, &string, 0);
		if (*tmp)
			push(&string, *tmp++);
	}
	return (string.string);
}
