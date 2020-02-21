/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_operator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 04:40:19 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/21 00:52:26 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char *expand_defaul_val(char *var_name, char *expression)
{
    t_variables *var;

    if ((var = get_var(var_name)) && !ft_strequ(var->value, ""))
        return (ft_strdup(var->value));
    return(expand(ft_strdup(expression + 2), NULL));
}

char *expand_alternative_val(char *var_name, char *expression)
{
    t_variables *var;

    if ((var = get_var(var_name)) && !ft_strequ(var->value, ""))
        return (expand(ft_strdup(expression + 2), NULL));
    return(ft_strdup(""));
}

char *assign_default_val(char *var_name, char *expression)
{
    char *val;
    t_variables *var;

    if (!(var = get_var(var_name)) || ft_strequ(var->value, ""))
    {
        if(!(val = expand(ft_strdup(expression + 2), NULL)))
            return(NULL);
        edit_add_var(var_name, val, 0, 0);
        free(val);
    }
    if ((var = get_var(var_name)))
        return (ft_strdup(var->value));
    return(NULL);
}

char *indicate_error(char *var_name, char *expression)
{
    char *val;
    t_variables *var;

    if ((var = get_var(var_name)) && !ft_strequ(var->value, ""))
        return (ft_strdup(var->value));
    else 
        val = expand(ft_strdup(expression + 2), NULL);
    ft_printf_fd(2, "%s : %s\n", var_name, val ? val : "");
    return (NULL);
}