/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 19:45:22 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/20 07:51:39 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char *expand_string_lenght(char *var_name, char *expression)
{
    t_variables *var;

    if(*expression)
        return (NULL);
    if ((var = get_var(var_name + 1)))
        return (ft_itoa(ft_strlen(var->value)));
    return(ft_strdup("0"));
}

char *expand_simple(char *var_name)
{
    t_variables *var;

    if ((var = get_var(var_name)) && !ft_strequ(var->value, ""))
        return (ft_strdup(var->value));
    return(ft_strdup(""));
}