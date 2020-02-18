/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 13:46:48 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/18 17:29:04 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
**	event not found function
*/

void		not_found_event(const char *name)
{
	ft_printf_fd(2, "42sh: %s: event not found\n", name);
	ERRNO = EVENTE;
}

void		command_line_n(const char **str, char c)
{
	const char *new;

	new = *str;
	if (*new)
	{
		if (ft_isdigit(*new))
			while (*new && ft_isdigit(*new))
				new++;
		else
			while (*new && !ft_isspace(*new) &&
					!is_metacharacter(*new) && *new != c)
				new++;
	}
	*str = new;
}

/*
**	Now that we get our events form the history
**  We have to swallow up all the chars in that events
*/

void		feed_t_string(char *event, t_string *str)
{
	char *new_line;

	if (event == NULL || *event == '\0')
		return (push(str, '!'));
	new_line = search_history(event + 1);
	if (new_line == NULL || *new_line == '\0')
		return (not_found_event(event));
	while (*new_line)
	{
		push(str, *new_line);
		new_line++;
	}
}

/*
**	This will call consume_word function
** to gulp all the charachters up the '\0' || '"' || metacharacter
*/

int			get_event(const char **new, t_string *str, char c)
{
	char	*new_string;

	if (is_metacharacter(**new))
	{
		not_found_event("!");
		return (0);
	}
	new_string = consume_word(new, c);
	feed_t_string(new_string, str);
	ft_strdel(&new_string);
	return (1);
}
