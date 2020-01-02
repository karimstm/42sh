/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 16:03:33 by amoutik           #+#    #+#             */
/*   Updated: 2019/12/31 10:08:14 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"
#include "globing.h"

static int			is_special_token(t_token_list *list, char **ptr,
					t_string *str, enum e_token_type *type)
{
	if (**ptr == '&' && !(*type & SH_REDIRECTION))
		return (0);
	if (str->len == 1 && ft_isdigit(*((*ptr) - 1)) &&
			!(*type & SH_REDIRECTION) && is_special_char(**ptr))
	{
		*type = (*type | SH_REDIRECTION);
		while (**ptr && is_special_char(**ptr))
			push(str, *(*ptr)++);
		insert_token(list, str, *type);
		*type = *type & ~SH_REDIRECTION;
		if (**ptr && !ft_isspace(**ptr) && !is_special_char(**ptr))
			return (1);
		if (**ptr && is_special_char(**ptr))
			return (0);
	}
	if (ft_strchr(SH_TOKEN, **ptr))
		return (0);
	return (1);
}

/*
	Check if a string is Glob
*/

int					is_globing(char **ptr,
								 enum e_token_type *type)
{
	if (**ptr && (**ptr == '*' || **ptr == '[' || **ptr == '?' || **ptr == ']'))
		*type = SH_GLOBE;
	return 1;
}

int					split_tok(t_token_list *list,
					char **ptr, t_string *str, enum e_token_type type)
{

	while (**ptr && !ft_isspace(**ptr))
	{
		if (!is_special_token(list, ptr, str, &type))
			break ;
		is_globing(ptr, &type);
		if (**ptr == '\\')
		{
			push(str, *(*ptr));
			if (*(++(*ptr)) != EOS)
				push(str, *(*ptr)++);
		}
		else if (**ptr == '\'' || **ptr == '"' || **ptr == '`')
		{
			if (split_quote(list, ptr, str, type | SH_QUOTED))
				return (0);
		}
		else if ((**ptr && **ptr == '$' && handle_dollar(ptr, str)) ||
				(**ptr && **ptr == '~' && handle_tilda(ptr, str)))
			;
		else if (**ptr && !ft_isspace(**ptr))
			push(str, *(*ptr)++);
	}
	if(fnmatch("[a-zA-Z]*=[a-zA-Z0-9]*", str->string, 0) == 0 && 
		(list->node_count == 0 || ft_strcmp(list->head->token, "export") == 0))
		type = type | SH_EXPORT;
	insert_token(list, str, type);
	return (1);
}

static int			split_special(t_token_list *list,
					char **ptr, t_string *str)
{
	while (**ptr == '|')
		push(str, *(*ptr)++);
	if (str->len == 1)
		return (insert_token(list, str, SH_PIPE));
	else if (str->len == 2)
		return (insert_token(list, str, SH_DPIPE));
	while (**ptr == ';')
		push(str, *(*ptr)++);
	if (str->len)
		return (insert_token(list, str, SH_SEMI));
	while (**ptr && is_special_char(**ptr))
		push(str, *(*ptr)++);
	insert_token(list, str, SH_REDIRECTION);
	if (split_tok(list, ptr, str, SH_WORD))
		return (1);
	return (0);
}

static int			stringtok(const char *line, t_token_list *list)
{
	char		*ptr;
	t_string	str;

	ptr = (char *)line;
	str.string = NULL;
	new_string(&str);
	while (1)
	{
		if (ptr == NULL || *ptr == EOS)
			break ;
		while (ft_isspace(*ptr))
			ptr++;
		if (*ptr &&
				(*ptr == '\'' || *ptr == '"' || *ptr == '`'))
		{
			if (split_quote(list, &ptr, &str, SH_WORD | SH_QUOTED))
				return (ret_with_str_free(&str, 0));
		}
		else if (*ptr && !ft_isspace(*ptr) && !ft_strchr(SH_TOKEN, *ptr))
		{
			if (!split_tok(list, &ptr, &str, SH_WORD))
				return (ret_with_str_free(&str, 0));
		}
		else if (*ptr && ft_strchr(SH_TOKEN, *ptr))
			if (!split_special(list, &ptr, &str))
				return (ret_with_str_free(&str, 0));
	}
	return (ret_with_str_free(&str, 1));
}

// Handle Globing function
// should be moved later on to another file for the norm

void				handle_globing(t_token_list *list)
{
	t_token *current;
	t_glob g;
	g.gl_pathc = 0;
	size_t i = 0;

	current = NULL;
	if (list && list->head)
		current = list->head;
	while (current)
	{
		if (current->tok_type == SH_GLOBE)
		{
			_glob(current->token, GLOB_DOOFFS, &error_func, &g);
			while (i < g.gl_pathc)
			{
				current = insert_token_indexed(current, g.gl_pathv[i], SH_WORD | SH_GLOBE);
				i++;
			} 
		}
		current = current->next;
	}
}

/* Remove slashes from string and re-assign it to token structure */

char				*remove_slashes(char *token)
{
	t_string str;
	str.string = NULL;
	char *tmp;

	new_string(&str);
	tmp = token;
	while(*token)
	{
		if (*token == '\\')
			token++;
		push(&str, *token);
		token++;
	}
	free(tmp);
	return str.string;
}		

/*
** (initial call) Remove slashes once you finished parsing
*/

void				init_remove_slashes(t_token_list *list)
{

	t_token *current;

	if (list->head)
		current = list->head;
	else
		return ;
	while (current)
	{
		if (current->tok_type == SH_WORD)
			current->token = remove_slashes(current->token);
		current = current->next;
	}

}

t_token_list		*handle_quote(char **line)
{
	t_token_list	*list;
	char			*ptr;

	ptr = *line;
	if ((list = (t_token_list *)malloc(sizeof(t_token_list))) == NULL)
		return (NULL);
	init_token_list(list);
	while (!stringtok(ptr, list) || is_pipe_at_end(list))
	{
		if (!check_quote_ending(&ptr))
		{
			parse_error(NULL, 2);
			free_token_list(list);
			return (NULL);
		}
		free_tokens(list);
	}
	//Handle Globing in here
	handle_globing(list);
	init_remove_slashes(list); // This will ensure no slash is missed
	if (check_syntax_error(list))
		return (NULL);
	// print_tokens(list);
	*line = ptr;
	return (list);
}
