/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arithmetic_expansion_parse.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 04:17:51 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/20 07:15:25 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arithmetic.h"

t_list *parse_ar_expression(char **expression, t_list *token);

void display_tokens(t_list *tokens)
{
    ft_printf("\n---------------------------------\n");
    while (tokens)
    {
        ft_printf("%s::", tokens->content);
        tokens = tokens->next;
    }
    ft_printf("\n---------------------------------");
}

void free_list0(void *ptr, size_t size)
{
    (void)size;
    free(ptr);
}

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
    while (ft_isalnum(**expr))
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

int get_sign(char **expression)
{
    int sign;

    sign = 1;
    while (1)
    {
        *expression = ft_skip_chars(*expression, " ", NULL);
        if (!ft_isinstr(**expression, "+-"))
            break;
        else if (expression[0][1] == **expression &&
                 ft_isalpha(*ft_skip_chars(*expression + 2, " ", NULL)))
            break;
        sign *= (44 - *(*expression)++); //magic number
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
        ft_lstdel(&token, free_list0);
    // display_tokens(token);
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
        ft_lstdel(&token, free_list0);
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
    ft_lstdel(token, free_list0);
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
        ft_lstdel(&token, free_list0);
    return (token);
}

// t_list *tokeniz_expression(char *expression)
// {
//     t_list *tokens;
//     t_list *tmp;
//     tokens = NULL;
//     while (*expression)
//     {
//         expression = ft_skip_chars(expression, " ", NULL);
//         if (!(tmp = parse_ar_expression(&expression, NULL)))
//         {
//             ft_lstdel(&tokens, free_list0);
//             ft_printf("parse error at '%.15s...'\n", expression);
//             return (NULL);
//         }
//         ft_lstenqueue(&tokens, tmp);
//     }
//     return (tokens);
// }

// int main(int ac, char *av[])
// {
//     t_list *tokens;
//     long long result;
//     int status;

//     result = 0;
//     ft_printf("argc = %d\n", ac);
//     if (ac == 2)
//     {
//         //tokens = tokeniz_expression(ft_strdup("  9==0-78----78+5/((07 - 989++78-7)* 78 +(ko   --  + hja  ++)) + 9    "));
//         tokens = tokeniz_expression(ft_strdup(av[1]));
//         if ((status = eval_expr(&tokens, &result)))
//         {
//             if(status == 2)
//                 ft_printf("divised by zero\n");
//             else
//                 ft_printf("token error: %s\n", tokens ? tokens->content : NULL);
//         }
//         else
//             ft_printf("result : %d\n", result);
//     }
//     // ft_printf("end");
//     // display_tokens(tokens);
//     return 0;
// }
