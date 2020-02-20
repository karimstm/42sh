/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluation_core.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 05:17:28 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/20 07:19:17 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arithmetic.h"

static char *operetors[14] = {"||", "&&", "!=", "==", ">", "<", ">=", "<=",
							  "-", "+", "%", "/", "*", 0};

// static int convert_variable(t_list **token, long long *result)
// {
// 	int prefix;
// 	int postfix;
// 	char *tmp;
// 	t_variables *var;

// 	prefix = 0;
// 	postfix = 0;
// 	if (ft_isinstr(((char *)(*token)->content)[0], "-+"))
// 		prefix = 44 - ((char *)(*token)->content)[0]; //ascii
// 	if (prefix)
// 		(*token)->content += 2;
// 	tmp = ft_skip_chars((*token)->content, NULL, ft_isalnum);
// 	if (!prefix && ft_isinstr(*tmp, "+-"))
// 	{
// 		postfix = 44 - *tmp;
// 		*tmp = 0;
// 	}
// 	if ((var = get_var((*token)->content) && !is_number(var->value)))
// 		return (1);
// 	*result = (var ? ft_atoi(var->value) : 0) + prefix;
// 	edit_add_var((*token)->content, ft_itoa(*result + postfix));
// 	return (0);
// }

long long convert_operand(t_list **token, long long *result)
{
	int sign;
	int error;

	if (!*token)
		return (1);
	sign = 1;
	error = 0;
	if (ft_strequ((*token)->content, "-") && (sign = -1))
		*token = (*token)->next;
	if (ft_strequ((*token)->content, "(") && ((*token = (*token)->next) || 1))
		error = eval_expr(token, result);
	else if (ft_isdigit(*((char *)(*token)->content)))
	{
		if (ft_strnequ((*token)->content, "0x", 2))
			*result = ft_atoi_base((*token)->content + 2, HEX_BASE, 16);
		else if (ft_strnequ((*token)->content, "0", 1))
			*result = ft_atoi_base((*token)->content, OCTAL_BASE, 8);
		else
			*result = ft_atoi((*token)->content);
	}
	// else
	// 	error = convert_variable(token, result);
	*token ? (*token = (*token)->next) : 0;
	*result *= sign;
	return (error);
}

int convert_operator(t_list **token, int *result, int skip_token)
{
	static char *opertrs[14] = {"||", "&&", "!=", "==", ">", "<", ">=", "<=",
								"-", "+", "%", "/", "*", 0};
	int i;

	i = 0;
	if (!*token)
		return (1);
	while (opertrs[i])
	{
		if (ft_strnequ((*token)->content, opertrs[i], ft_strlen(opertrs[i])))
			break;
		i++;
	}
	if (BETWEEN(i, 0, 12))
	{
		if (skip_token)
			*token = (*token)->next;
		*result = i + 1;
		return (0);
	}
	return (1);
}

int eval_expr(t_list **tokens, long long *result)
{
	int has_error;
	int operator;
	long long operand2;

	has_error = convert_operand(tokens, result);
	while (*tokens && !has_error && !ft_strequ((*tokens)->content, ")"))
	{
		has_error = convert_operator(tokens, &operator, 1);
		if (!has_error && (operator== 1 || operator== 2))
		{
			has_error = do_logical_op(tokens, result, operator);
			continue;
		}
		if (operator_have_precedence(tokens, operator))
			has_error = convert_operand(tokens, &operand2);
		else
			has_error = eval_expr(tokens, &operand2);
		ft_printf("%d %s %d\n",*result, operetors[operator - 1], operand2);
		has_error = do_op(result, operator, operand2, 0);
	}
	return (has_error);
}

char *expand_ar_expr(char *expr)
{
	// need to free token
	t_list *tokens;
	long long result;
	int status;

	result = 0;
	tokens = parse_ar_expression(&expr, NULL);
	if ((status = eval_expr(&tokens, &result)))
	{
		if (status == 2)
			ft_printf("divised by zero\n");
		else
			ft_printf("token error: %s\n", tokens ? tokens->content : NULL);
		return (NULL);
	}
	return (ft_itoa(result));
}