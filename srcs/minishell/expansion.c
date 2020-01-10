/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 18:33:49 by amoutik           #+#    #+#             */
/*   Updated: 2020/01/08 18:45:48 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	parameter_parser(char *string)
{

}

void	parameter_expension(t_list_simple_command *list)
{
	t_simple_command	*current;
	char				*string;
	char				*start;

	current = list->head ? list->head : NULL;
	while (current)
	{
		if ((string = strchr(current->name, '$')))
		{
			string++;
			if (*string == '{')
			{
				start = string + 1;
				while (*string && *string != '}')
					string++;
			}
		}
	}
}
