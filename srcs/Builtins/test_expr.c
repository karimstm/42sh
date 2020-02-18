/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 14:42:58 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/18 17:13:20 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_token_op	parse_token_op(char *argv)
{
	char		*tmp;
	int			error;
	t_token_op	op;

	op = TOKEN_OP_NONE;
	error = 0;
	tmp = argv;
	if (*tmp == '-')
	{
		tmp++;
		if ((op = get_token_op(tmp)) == TOKEN_OP_NONE)
			error = 1;
	}
	else if (ft_strequ(tmp, "="))
		op = TOKEN_OP_STRING_EQ;
	else if (ft_strequ(tmp, "!="))
		op = TOKEN_OP_STRING_NE;
	else if (error)
		return (ft_op_error(argv));
	else
		return (ft_op_error(argv));
	return (op);
}

int			ft_isnumber(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int			evaluate_expr_int(t_token_op op, int rval, int lval)
{
	if (op == TOKEN_OP_EQ)
		return (rval == lval);
	if (op == TOKEN_OP_GE)
		return (rval >= lval);
	if (op == TOKEN_OP_GT)
		return (rval > lval);
	if (op == TOKEN_OP_LE)
		return (rval <= lval);
	if (op == TOKEN_OP_LT)
		return (rval < lval);
	if (op == TOKEN_OP_NE)
		return (rval != lval);
	return (0);
}

int			evaluate_expr(t_test *test)
{
	int		rval;
	int		lval;
	int		sign;
	char	*str;

	sign = 0;
	if (test->op == TOKEN_OP_STRING_NE)
		return (!ft_strequ(test->s1, test->s2));
	if (test->op == TOKEN_OP_STRING_EQ)
		return (ft_strequ(test->s1, test->s2));
	str = test->s1;
	if (*str == '-' || *str == '+')
		sign = *str++;
	if (ft_isnumber(str))
		rval = ft_atoi(test->s1);
	else
		return (ft_integer_error(test->s1));
	str = test->s2;
	if (*str == '-' || *str == '+')
		sign = *str++;
	if (ft_isnumber(str))
		lval = ft_atoi(test->s2);
	else
		return (ft_integer_error(test->s2));
	return (evaluate_expr_int(test->op, rval, lval));
}

int			expression(char **argv)
{
	t_test		test;
	t_token_op	op;

	test.s1 = argv[0];
	if ((op = parse_token_op(argv[1])) != TOKEN_OP_NONE)
		test.op = op;
	else
		return (-1);
	test.s2 = argv[2];
	return (evaluate_expr(&test));
}
