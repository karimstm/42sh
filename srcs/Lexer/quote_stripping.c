/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_stripping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:25:04 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 16:25:05 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char		*quote_stripping(char *str)
{
	t_string	string;
	char		*tmp;

	string.string = NULL;
	new_string(&string);
	tmp = str;
	while (*tmp)
	{
		if (*tmp == '\\')
			tmp++;
		else if (*tmp == '"' || *tmp == '\'')
			tmp++;
		else if (*tmp)
			push(&string, *tmp++);
	}
	return (string.string);
}
