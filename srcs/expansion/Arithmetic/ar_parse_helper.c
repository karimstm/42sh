/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ar_parse_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/21 04:45:21 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/21 04:50:57 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arithmetic.h"

t_list *parse_number(char **expression)
{
    char *tmp;
    t_list *token;

    tmp = *expression;
    if (ft_strnequ(*expression, "0x", 2))
        *expression = ft_skip_chars(*expression + 2, "0123456789abcdef", NULL);
    else if (**expression == '0')
        *expression = ft_skip_chars(*expression, "01234567", NULL);
    else
        *expression = ft_skip_chars(*expression, NULL, ft_isdigit);
    tmp = ft_strsub(tmp, 0, *expression - tmp);
    token = ft_lstnew(tmp, ft_strlen(tmp) + 1);
    free(tmp);
    return (token);
}

t_list *parse_variable(char **expr)
{
    char *tmp;
    t_list *token;
    int i;
    int j;

    i = 0;
    tmp = *expr;
    while (ft_isinstr(**expr, "+-"))
        tmp[i++] = *(*expr)++;
    *expr = ft_skip_chars(*expr, " ", NULL);
    while (ft_isalnum(**expr) || **expr == '_')
        tmp[i++] = *(*expr)++;
    *expr = ft_skip_chars(*expr, " ", NULL);
    if (ft_isinstr(**expr, "+-") && **expr == expr[0][1] && (j = i))
        while (i - j < 2)
            tmp[i++] = *(*expr)++;
    if (i == 0)
        return (NULL);
    tmp = ft_strsub(tmp, 0, i);
    token = ft_lstnew(tmp, i + 1);
    free(tmp);
    return (token);
}