/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 16:03:34 by amoutik           #+#    #+#             */
/*   Updated: 2019/05/14 17:31:10 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int				check_quote_ending(char **line)
{
	char	*ptr;
	int		flag;
	t_line	*new_line;

	flag = 1;
	ptr = ft_strdup(*line);
	free_line();
	new_line = init_line();
	new_line->print_msg = 0;
	read_line(new_line);
	if (new_line->print_msg || new_line->command[new_line->index] == EOT_KEY)
		ptr[0] = '\0';
	if (new_line->command[new_line->index] == EOT_KEY)
		flag = 0;
	*line = ft_strjoin(ptr, (new_line->print_msg) ? "\0" : "\n");
	ft_strdel(&ptr);
	ptr = *line;
	*line = ft_strjoin(ptr, new_line->command);
	ft_strdel(&ptr);
	ft_strdel(&new_line->command);
	new_line->command = *line;
	return (flag);
}

static int		is_special_token(char **ptr,
		t_string *str, int inquote, enum e_token_type *type)
{
	if (!inquote && **ptr == '&' && !(*type & SH_REDIRECTION))
		return (0);
	if (!inquote && str->len == 1 && ft_isdigit(*((*ptr) - 1)) &&
			!(*type & SH_REDIRECTION) && is_special_char(**ptr))
	{
		*type = SH_REDIRECTION;
		while (**ptr && is_special_char(**ptr))
			push(str, *(*ptr)++);
		while (**ptr && !ft_isspace(**ptr) && !is_special_char(**ptr))
			push(str, *(*ptr)++);
		if (**ptr && is_special_char(**ptr))
			return (0);
	}
	if (!inquote && ft_strchr(SH_TOKEN, **ptr))
		return (0);
	return (1);
}

static void		escaped_char(char **ptr, t_string *str)
{
	char	escaped;

	if (*((*ptr) + 1) != EOS)
	{
		if ((escaped = space(*((*ptr) + 1))))
			push(str, escaped);
		if (escaped == 0)
			push(str, *(++(*ptr)));
		else
		{
			// push(str, *(*ptr));
			(*ptr)++;
		}
	}
}

static int		is_quote(char c)
{
	if (c == '\'' || c == '"' || c == '`')
		return (1);
	return (0);
}

int				split_quote(t_token_list *list, char **ptr,
		t_string *str, enum e_token_type	type)
{
	int				inquote;
	char			quote;

	inquote = 1;
	quote = *(*ptr)++;
	while (**ptr &&
			(inquote || (!ft_isspace(**ptr) && !inquote)))
	{
		if (!is_special_token(ptr, str, inquote, &type))
			break ;
		if (**ptr == '\\')
			escaped_char(ptr, str);
		else if (!inquote && is_quote(**ptr) && ++inquote)
			quote = **ptr;
		else if (**ptr == '$' && quote != '\'' && handle_dollar(ptr, str))
			continue;
		else if (**ptr && **ptr != quote)
			push(str, *(*ptr));
		else if (**ptr == quote && !(quote = 0))
			inquote = 0;
		++(*ptr);
	}
	if (!inquote)
		insert_token(list, str, type);
	return (inquote);
}
