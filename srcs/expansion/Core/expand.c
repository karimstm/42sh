/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 20:27:30 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/20 07:30:18 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static char *join_expan_result(t_parser_expansion result, char **str)
{
	char *tmp;
	char *ret;

	tmp = *str;
	if((*str = ft_strnjoin((char*[]){tmp, result.str, result.index}, 3)))
	{
		ret = *str + ft_strlen(result.str) + ft_strlen(tmp);
		free(tmp);
		free(result.str);
		return(ret);
	}
	ft_strdel(str);
	free(result.str);
	return(NULL);
}

char *expand(char *str, t_parser_expansion (*expand_fun)(char *))
{
	char *tmp;
	int qoute;
	t_parser_expansion result;

	tmp = str;
	qoute = 0;
	while (*tmp)
	{
		if (*tmp == '\\')
			tmp++;
		else if (!qoute && *tmp == '$' && tmp[1] && !ft_strchr(" \t\n", tmp[1]))
		{
			expand_fun =  tmp[1] == '(' ? expand_sub_art : expand_parametre;
			result = expand_fun(&tmp[1]);
			*tmp = 0;
			if (!(tmp = join_expan_result(result, &str)))
				return(NULL);
			continue;
		}
		else if (*tmp == '\'')
			qoute ^= 1;
		tmp++;
	}
	return(str);
}

int expand_args(t_list_simple_command *args)
{
	t_simple_command *ptr;

	ptr = args->head;
	while (ptr)
	{
		//expand tilde first
		if (!(ptr->name = expand(ptr->name, NULL)))
			return (0);
		ptr = ptr->next;
	}
	return(1);
}