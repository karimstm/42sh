/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arithmetic_expansion_parse.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 04:17:51 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/07 02:39:38 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list *parse_ar_expression(char **expression);

void free_list(void *ptr, size_t size)
{
	free(ptr);
}

t_list *parse_number(char **expression)
{
    char *tmp;
    t_list *token;

	tmp = *expression;
    if (ft_strnequ(*expression, "0x", 2))
        *expression = *expression + 2;
    *expression = ft_skip_chars(*expression, NULL, ft_isdigit);
    tmp = ft_strsub(tmp, 0, *expression - tmp);
    token = ft_lstnew(tmp, *expression - tmp + 1);
    free(tmp);
    return(token);
}

t_list *parse_variable(char **expression)
{
    char *tmp;
    t_list *token;
	int i;

	i = 0;
	tmp = *expression;
	while (ft_isinstr(**expression, "+-"))
		tmp[i++] = *(*expression++);
	*expression = ft_skip_chars(*expression, " ", NULL);
    while (ft_isalnum(**expression))
		tmp[i++] = expression[0][0]++;
    *expression = ft_skip_chars(*expression, " ", NULL);
	if (ft_isinstr(**expression, "+-") && **expression == expression[0][1] && (i += 2))
		ft_strncpy(tmp + i, *expression, 2);
    if (i == 0)
    {
        //print error;
        return(NULL);
    }
    tmp = ft_strsub(tmp, 0, i);
    token = ft_lstnew(tmp, i + 1);
    free(tmp);
    return(token);
}

int get_sign(char	**expression)
{
    int sign;

    sign = 1;
    while (1)
    {
        *expression = ft_skip_chars(*expression, " ", NULL);
        if (!ft_isinstr(**expression, "+-"))
            break;
        else if(expression[0][1] == **expression &&
                            ft_isalnum(*ft_skip_chars(*expression, " ", NULL)))
            break;
        sign *= (44 - *(*expression++)); //magic number
    }
	return(sign);
}

t_list *get_operand(char **expression, t_list *token)
{
    t_list *ptr;

	if(get_sign(expression) == -1)
		ptr = ft_lstnew("-", 2);;
    if (**expression == '(')
        ft_lstenqueue(&ptr, parse_ar_expression(expression));
    else if (ft_isdigit(**expression))
        ft_lstenqueue(&ptr, parse_number(expression));
    else
        ft_lstenqueue(&ptr, parse_variable(expression));
    ft_lstenqueue(&token, ptr);
    if(!ptr)
        ft_lstdel(&token, free_list);
    return (token);
}

t_list *get_operator(char **expression, t_list *token)
{
    static char *operators[14] = {"||", "&&", "!=", "==", ">", "<", ">=", "<=", "-", "+", "%", "/", "*", 0};
	int i;
    t_list *ptr;
    
    ptr = NULL;
	i = 0;
    *expression = ft_skip_chars(*expression, " ", NULL);
    while (*operators)
    {
        if (ft_strnequ(*expression, operators[i], ft_strlen(operators[i])))
        {
            *expression = ft_skip_chars(*expression + ft_strlen(operators[i]), " ", NULL);
            ptr = (ft_lstnew(operators[i], ft_strlen(operators[i]) + 1));
			break;
        }
        i++;
    }
    ft_lstenqueue(&token, ptr);
    if (!ptr)
		ft_lstdel(&token, free_list);
    return (token);
}

t_list *parse_ar_expression(char **expression)
{
    t_list *token;
    char c;

    token = NULL;
    c = (**expression == '(' ? ')' : 0);
    if (c == ')' && *++(*expression) != ')')
        token = ft_lstnew((char[]){'(', 0}, 2);
    while (**expression != c && **expression)
    {
        if (!(token = get_operand(expression, token)))
            return (NULL);
        if (**expression && !(token = get_operator(expression, token)))
            return (NULL);
    }
    if (**expression == ')' && token)
        token = ft_lstnew((char[]){')', 0}, 2);
    else if (**expression == ')')
        return (NULL);
	return(token);
}

t_list *tokeniz_expression(char *expression)
{
    t_list *tokens;
    while (*expression)
    {
        parse_ar_expression(&expression);
    }
    return(tokens);
}

int main()
{
    t_list *tokens;
    
    tokens = tokeniz_expression("");
    return 0;
}
