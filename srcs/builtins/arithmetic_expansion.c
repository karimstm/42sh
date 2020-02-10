/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arithmetic_expansion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 05:17:28 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/04 22:30:56 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#define BETWEEN(v, v1, v2) ((v) >= (v1) && (v) <= (v2))

static do_op(long op1, int oparator, long op2)
{
    if (oparator == 13)
        return (op1 * op2);
    if (oparator == 12)
        return (op1 / op2);
    if (oparator == 11)
        return (op1 % op2);
    if (oparator == 10)
        return (op1 + op2);
    if (oparator == 9)
        return (op1 - op2);
    if (oparator == 8)
        return (op1 <= op2);
    if (oparator == 7)
        return (op1 >= op2);
    if (oparator == 6)
        return (op1 < op2);
    if (oparator == 5)
        return (op1 > op2);
    if (oparator == 4)
        return (op1 == op2);
    if (oparator == 3)
        return (op1 != op2);
    return (op1);
}

int get_precedence(int operator)
{
    if (BETWEEN(operator, 11, 13))
        return (5);
    if (BETWEEN(operator, 10, 9))
        return (4);
    if (BETWEEN(operator, 8, 5))
        return (3);
    if (BETWEEN(operator, 4, 3))
        return (2);
    if (BETWEEN(operator, 2, 1))
        return (1);
    return (0);
}

t_list *split_expression_rpn(char *expression)
{
    while (expression)
    {
    }
}

static long parse_token(t_list *token)
{
    if (token)
    {
        if (*((char *)token->content) == '(')
            return (eval_expr(split_expression(token->content)));
        if (*((char *)token->content) == '(')
            return ();
    }
    return (0);
}

long eval_expr(t_list **tokens)
{
    int operator;
    long operand1;
    long operand2;

    operand1 = parse_token(*tokens);
    *tokens = (*tokens)->next;
    while (tokens)
    {
        operator= parse_token((*tokens));
        if (operator== 1 || operator== 2)
        {
            operand1 = do_logical_op(operand1, operator, tokens);
            continue;
        }
        if (get_precedence(parse_token((*tokens)->next->next)) <= get_precedence(operator))
            operand2 = eval_expr(tokens);
        else
        {
            operand2 = parse_token((*tokens)->next);
            *tokens = (*tokens)->next;
        }
        operand1 = do_op(operand1, operator, operand2);
    }
    return (operand1);
}

t_list *get_operand(char **expression, t_list *token)
{
    int sign;
    t_list *ptr;

    sign = 1;
    while (1)
    {
        *expression = ft_skip_chars(" ");
        if (!ft_isinstr(**expression, "+-"))
            break;
        sign *= (44 - *(*expression)++);//magic number
    }
    if (**expression == '(')
        ptr = parse_ar_expression(expression);
    else if(**expression == '0')
        ptr = parse_number(expression);
    else
        ptr = parse_variable(expression);
    token ? token->next = ptr :  0;
    return(token ? token : ptr);
}

t_list *parse_ar_expression(char **expression)
{
    t_list *token;

    token = NULL;
    while (*expression)
    {
            if (!(token = get_operand(expression, token)))
                return (NULL);
            else if (!(token = get_operator(expression, token)))
                return (NULL);
        i = (i + 1) % 3;
    }
}