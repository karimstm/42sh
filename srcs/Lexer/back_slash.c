/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   back_slash.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 13:51:33 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/29 13:51:51 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int						back_slash_escape(char **string,
						t_string *str, int include)
{
	char *new;

	new = *string;
	if (!include && *(new + 1) == '\\')
		new++;
	else if (*(new + 1) != '\n')
		push(str, *new++);
	else
	{
		*(++new) ? ++new : 0;
		*string = new;
		return (1);
	}
	*string = new;
	return (0);
}
