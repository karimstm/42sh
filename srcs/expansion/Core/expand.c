/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 20:27:30 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/25 12:06:29 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static char *expand_tilde(char *str)
{
	t_variables *var;
	char *tmp;

	if (str && *str == '~')
	{
		if (str[1] == '/' || str[1] == '\0')
		{
			var = get_var("HOME");
			tmp = ft_strjoin(var ? var->value : "", str + 1);
			free(str);
			str = tmp;
		}
		else if (getpwnam(str + 1))
		{
			tmp = ft_strjoin("/Users/", str + 1);
			free(str);
			str = tmp;
		}
	}
	return (str);
}

static char *join_expan_result(t_parser_expansion result, char **str)
{
	char *tmp;
	char *ret;

	tmp = *str;
	// ft_printf("|%|", tmp);
	if ((*str = ft_strnjoin((char *[]){tmp, result.str, result.index}, 3)))
	{
		ret = *str + ft_strlen(result.str) + ft_strlen(tmp);
		free(tmp);
		free(result.str);
		return (ret);
	}
	ft_strdel(str);
	free(result.str);
	return (NULL);
}

char *expand(char *str, t_parser_expansion (*expand_fun)(char *))
{
	char *tmp;
	int qoute;
	t_parser_expansion result;

	str = expand_tilde(str);
	tmp = str;
	qoute = 0;
	while (tmp && *tmp)
	{
		if (*tmp == '\\')
			tmp++;
		else if (!qoute && *tmp == '$' && tmp[1] && !ft_strchr(" \t\n", tmp[1]))
		{
			expand_fun = tmp[1] == '(' ? expand_sub_art : expand_parametre;
			result = expand_fun(&tmp[1]);
			*tmp = 0;
			if (!(tmp = join_expan_result(result, &str)))
				return (NULL);
			continue;
		}
		else if (*tmp == '\'')
			qoute ^= 1;
		tmp++;
	}
	return (str);
}

int expand_args(t_list_simple_command *args)
{
	t_simple_command *ptr;

	ptr = args && args->head ? args->head : NULL;
	while (ptr)
	{
		if (!ptr->name || !(ptr->name = expand(ptr->name, NULL)))
			return (0);
		ptr = ptr->next;
	}
	return (1);
}
