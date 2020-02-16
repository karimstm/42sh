/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 10:49:43 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 13:44:53 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
**	This function will make sure to conusme
** 	up to space \<new_line> or other IFS charachters
**	One thing to note here is that new_string need to be freed
**	in case history resolver function return a new allocated string
*/

char		*consume_word(const char **line, char c)
{
	size_t			len;
	const char		*str;
	char			*new_string;

	str = *line;
	str++;
	if (*str)
	{
		if (*str == '!')
			str++;
		else
		{
			if (*str == '-')
				str++;
			command_line_n(&str, c);
		}
	}
	len = str - *line;
	new_string = len != 1 ? ft_strsub(*line, 0, len) : NULL;
	*line = str;
	return (new_string);
}

/*
**	Consume the string up to where it =>
**	=> finds single quote or a null termintor
*/

void		consume_single_quote(const char **line, t_string *tstring)
{
	const char	*str;
	char		c;

	str = *line;
	push(tstring, *str++);
	while ((c = *str))
	{
		push(tstring, c);
		str++;
		if (c == '\'')
			break ;
	}
	*line = str;
}

/*
**	 There some special cases with event inside
**	 double quote, therefore we had to implemented on it's own
*/

void		consume_double_quote(const char **line, t_string *str)
{
	const char	*new;

	new = *line;
	push(str, *new++);
	while (*new && *new != '"')
	{
		if (*new == '\\')
		{
			push(str, *new++);
			if (*new)
				push(str, *new);
		}
		else if (*new != '!')
			push(str, *new);
		else if (!get_event(&new, str, '"'))
			break ;
		if (*new)
			new++;
		*line = new;
	}
	if (*new == '"')
		push(str, *new++);
	*line = new;
}

/*
**	This function consume history keyword
**	! => !n => !-n => !! => !string
*/

int			consume_history(const char **pure_line, t_string *str)
{
	const char	*line;

	line = *pure_line;
	while (*line)
	{
		if (*line == '\'')
			consume_single_quote(&line, str);
		else if (*line == '"')
			consume_double_quote(&line, str);
		else if (*line == '\\' || *line == '[')
		{
			push(str, *line++);
			if (*line)
				push(str, *line);
		}
		else if (*line != '!')
			push(str, *line);
		else if (!get_event(&line, str, '\0'))
			break ;
		if (*line)
			line++;
		*pure_line = line;
	}
	return (0);
}

/*
**	Initial parser for history expansion
*/

char		*pre_parse(const char *line)
{
	t_string		string;
	const char		*start;

	ERRNO = 0;
	start = line;
	string.string = NULL;
	new_string(&string);
	consume_history(&line, &string);
	if (!ERRNO)
	{
		if (!ft_strequ(start, string.string))
			ft_printf("%s\n", string.string);
		return (string.string);
	}
	return (NULL);
}
