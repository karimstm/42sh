/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 10:49:43 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/13 13:54:11 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "shell.h"

char			*fake_word(const char *name)
{
	(void)name;
	return (strdup("fake name"));
}


void			command_line_n(const char **str)
{
	const char *new;

	new = *str;
	if (*new)
	{
		if (ft_isdigit(*new))
			while(*new && ft_isdigit(*new))
				new++;
		else
			while (*new)
				new++;
	}
	*str = new;
}

/*
**	This function will make sure to conusme
** 	up to space \<new_line> or other IFS charachters
**	One thing to note here is that new_string need to be freed
**	in case history resolver function return a new allocated string
*/

const char		*consume_word(const char **line)
{
	size_t		len;
	const char	*str;
	char		*new_string;

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
			command_line_n(&str);
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

void			consume_single_quote(const char **line)
{
	const char		*str;

	str = *(++line);
	while (*str && *str != '\'')
		str++;
	*line = *str ? (++str) : str;
}


void			feed_t_string(const char *event, t_string *str)
{
	char		*new_line;

	if (event == NULL || *event == '\0')
		return (push(str, '!'));
	new_line = fake_word(event);
	if (new_line == NULL || *new_line == '\0')
	{
		ft_printf_fd(2, "42sh: %s: event not found\n", event);
		ERRNO = EVENTE;
		return ;
	}
	while (*new_line)
	{
		push(str, *new_line);
		new_line++;
	}
}

/*
**	This function consume history keyword
**	! => !n => !-n => !! => !string
*/

int				consume_history(const char **pure_line, t_string *str)
{
	const char *line;

	line = *pure_line;
	while (*line)
	{
		if (*line != '!')
			push(str, *line);
		else
			feed_t_string(consume_word(&line), str);
		if (*line == '\'')
			consume_single_quote(&line);
		if (*line == '\\')
			line++;
		if (*line)
			line++;
		*pure_line = line;
	}
	return (0);
}

char			*pre_parse(const char *line)
{
	t_string	string;

	string.string = NULL;
	ERRNO = 0;
	new_string(&string);
	consume_history(&line, &string);
	if (!ERRNO)
		return (string.string);
	return (NULL);
}
