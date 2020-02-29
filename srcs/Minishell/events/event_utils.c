/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 12:16:23 by cjamal            #+#    #+#             */
/*   Updated: 2020/02/29 12:18:46 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			spacial_cases(const char *line)
{
	if (*line == '!' && *(line + 1) == '=')
		return (1);
	if (*line == '!' && ft_isspace(*(line + 1)))
		return (1);
	return (0);
}

int			quotes_s(const char **real_line, t_string *str)
{
	const char	*line;

	line = *real_line;
	if (*line == '\'')
	{
		consume_single_quote(&line, str);
		*real_line = line;
		return (1);
	}
	else if (*line == '"')
	{
		consume_double_quote(&line, str);
		*real_line = line;
		return (1);
	}
	return (0);
}
