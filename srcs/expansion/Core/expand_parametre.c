/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_parametre.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 00:12:20 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/23 02:23:02 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char *get_variable_name(char **str)
{
    int i;
    char *tmp;

    i = 0;
    if ((str[0][0] == '#' && str[0][1] == '?') || str[0][0] == '?')
        i = 1 + str[0][0] == '#';
    else if (ft_isinstr(**str, "#_") || ft_isalpha(**str))
    {
        i++;
        while (i && str[0][i] && (ft_isalnum(str[0][i]) || str[0][i] == '_'))
            i++;
    }
    if (i > 0)
    {
        tmp = ft_strsub(*str, 0, i);
        *str = *str + i;
        str[0][-1] = 0;
        return (tmp);
    }
    return (NULL);
}

char *dispach_expansion(char *var_name, char *expression)
{
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

t_parser_expansion expand_parametre(char *str)
{
    t_parser_expansion ret;
    const char *dup = str;
    char *tmp;
    char *var_name;

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
    ft_printf("<%s, %s>\n", var_name, str);
    if (!(ret.str = dispach_expansion(var_name, str)))
        ft_printf("${%s} : bad substitution", dup);
    free(var_name);
    return (ret);
}
