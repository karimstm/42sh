/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arithmetic_expansion_parse.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 04:17:51 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/21 12:07:25 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arithmetic.h"

int get_sign(char **expression)
{
    int sign;
    char *tmp;

    sign = 1;
    while (1)
    {
        *expression = ft_skip_chars(*expression, " ", NULL);
        if (!ft_isinstr(**expression, "+-"))
            break;
        else if (expression[0][1] == **expression)
        {
            tmp = ft_skip_chars(*expression + 2, " ", NULL);
            if (*tmp == '_' || ft_isalpha(*tmp))
                break;
        }
        sign *= (44 - *(*expression)++);
    }
    *expression = ft_skip_chars(*expression, " ", NULL);
    return (sign);
}

t_list *get_operand(char **expression, t_list *token)
{
    t_list *ptr;

    ptr = NULL;
    if (get_sign(expression) == -1)
        ft_lstenqueue(&token, ft_lstnew("-", 2));
    if (**expression == '(')
    {
        *expression = ft_skip_chars(*expression + 1, " ", NULL);
        ft_lstenqueue(&ptr, parse_ar_expression(expression, ft_lstnew("(", 2)));
    }
    else if (ft_isdigit(**expression))
        ft_lstenqueue(&ptr, parse_number(expression));
    else
        ft_lstenqueue(&ptr, parse_variable(expression));
    *expression = ft_skip_chars(*expression, " ", NULL);
    ft_lstenqueue(&token, ptr);
    if (!ptr)
        ft_lstdel2(&token, free);
    return (token);
}

t_list *get_operator(char **expr, t_list *token)
{
    static char *operators[14] = {"||", "&&", "!=", "==", ">=", "<=", ">",
                                  "<", "-", "+", "%", "/", "*", 0};
    int i;
    t_list *ptr;

    ptr = NULL;
    i = 0;
    *expr = ft_skip_chars(*expr, " ", NULL);
    while (operators[i])
    {
        if (ft_strnequ(*expr, operators[i], ft_strlen(operators[i])))
        {
            *expr = ft_skip_chars(*expr + ft_strlen(operators[i]), " ", NULL);
            ptr = (ft_lstnew(operators[i], ft_strlen(operators[i]) + 1));
            break;
        }
        i++;
    }
    ft_lstenqueue(&token, ptr);
    if (!ptr)
        ft_lstdel2(&token, free);
    return (token);
}

int get_parentese(char **expression, t_list **token)
{
    if (**expression == '(')
    {
        if (**expression == '(' && expression[0]++ [1] != ')')
            *token = ft_lstnew((char[]){'(', 0}, 2);
        return (')');
    }
    if (**expression == ')')
    {
        ft_lstenqueue(token, ft_lstnew((char[]){')', 0}, 2));
        (*expression)++;
        return (1);
    }
    ft_lstdel2(token, free);
    return (0);
}

t_list *parse_ar_expression(char **expression, t_list *token)
{
    int i;
    const int need_bracket_close = token && ft_strequ(token->content, "(");

    i = 0;
    *expression = ft_skip_chars(*expression, " ", NULL);
    while (**expression != ')' && **expression)
    {
        if (!(i % 2) && !(token = get_operand(expression, token)))
            return (NULL);
        else if ((i % 2) && !(token = get_operator(expression, token)))
            return (NULL);
        i = (i + 1) % 2;
    }
    if (need_bracket_close)
        get_parentese(expression, &token);
    if (i == 0)
        ft_lstdel2(&token, free);
    return (token);
}
