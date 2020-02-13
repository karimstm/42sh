/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 10:49:43 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/13 11:45:55 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "shell.h"

char			*fake_word(char *name)
{
	(void)name;
	return (strdup("fake name"));
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
	char	*new_string;

	new_string = NULL;
	str = *line;
	while (*str && !ft_isspace(*line))
		str++;
	len = str - *line;
	new_string = len ? ft_strsub(*line, 0, len) : NULL;
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
	line = *str ? (++str) : str;
}


void			feed_t_string(const char *)
{

}

/*
**	This function consume history keyword
**	! => !n => !-n => !! => !string
*/

int				consume_history(const char *line, t_string *str)
{
	while (*line)
	{
		if (*line != '!')
			push(str, *line);
		else
			consume_word(&line);
		if (*line == '\'')
			consume_single_quote(&line);
		if (*line == '\\')
			line++;
		if (*line)
			line++;
	}
	return (0);
}

char			*pre_parse(const char *line)
{
	t_string	string;

	string.string = NULL;
	new_string(&string);
	consume_history(line, &string);
}
