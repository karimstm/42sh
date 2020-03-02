/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_parametre.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 00:12:20 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/03/02 22:17:04 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static void			exp_parm_update_status(void)
{
	char	*tmp;

	tmp = ft_itoa(JOB_LIST->status);
	edit_add_var("_status", tmp, 0, 0);
	ft_strdel(&tmp);
	tmp = ft_itoa(getpid());
	edit_add_var("_pid", tmp, 0, 0);
	free(tmp);
}

static char			*get_variable_name(char **s)
{
	int		i;
	char	*tmp;

	i = 0;
	if ((**s == '#' && ft_isinstr(s[0][1], "?$")) || ft_isinstr(**s, "?$"))
		i = 1 + (s[0][0] == '#');
	else if (ft_isinstr(**s, "#_") || ft_isalpha(**s))
	{
		i++;
		while (i && s[0][i] && (ft_isalnum(s[0][i]) || s[0][i] == '_'))
			i++;
	}
	if (i > 0)
	{
		tmp = ft_strsub(*s, 0, i);
		*s = *s + i;
		s[0][-1] = 0;
		return (tmp);
	}
	return (NULL);
}

static char			*dispach_expansion(char *var_name, char *expression)
{
	var_name = ft_strstr(var_name, "?") ? "_status" : var_name;
	var_name = ft_strstr(var_name, "$") ? "_pid" : var_name;
	if (*var_name == '#')
		return (expand_string_lenght(var_name, expression));
	if (!*expression)
		return (expand_simple(var_name));
	if (ft_strnequ(expression, "##", 2))
		return (expand_rem_pre(var_name, expression, 0, ft_max));
	if (ft_strnequ(expression, "#", 1))
		return (expand_rem_pre(var_name, expression, LONG_MAX, ft_min));
	if (ft_strnequ(expression, "%%", 2))
		return (expand_rem_suf(var_name, expression, 0, ft_max));
	if (ft_strnequ(expression, "%", 1))
		return (expand_rem_suf(var_name, expression, LONG_MAX, ft_min));
	if (ft_strnequ(expression, ":-", 2))
		return (expand_defaul_val(var_name, expression));
	if (ft_strnequ(expression, ":+", 2))
		return (expand_alternative_val(var_name, expression));
	if (ft_strnequ(expression, ":?", 2))
		return (indicate_error(var_name, expression));
	if (ft_strnequ(expression, ":=", 2))
		return (assign_default_val(var_name, expression));
	return (NULL);
}

t_parser_expansion	expand_parametre(char *str)
{
	t_parser_expansion	ret;
	const char			*dup = str;
	char				*tmp;
	char				*var_name;

	var_name = NULL;
	ft_bzero(&ret, sizeof(t_parser_expansion));
	if ((tmp = get_matched_bracket(str, "(){}''\"\"", NULL, 1)) && *str == '{')
	{
		*tmp = 0;
		str++;
	}
	else if (*str == '{')
		return (ret);
	else
		tmp = NULL;
	var_name = get_variable_name(&str);
	ret.index = tmp ? tmp + 1 : str--;
	exp_parm_update_status();
	if (var_name && !(ret.str = dispach_expansion(var_name, str)))
		PRINT_ERROR2(dup, ": bad substitution");
	free(var_name);
	return (ret);
}
