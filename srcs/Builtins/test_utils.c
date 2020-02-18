/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 17:13:37 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/18 17:13:38 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int				ft_op_error(char *s)
{
	ft_printf_fd(2, "42sh: test: %s: binary operator expected\n", s);
	return (0);
}

int				ft_integer_error(char *s)
{
	ft_printf_fd(2, "42sh: test: %s: integer expression expected\n", s);
	return (-1);
}

t_token_op		get_token_op(char *tmp)
{
	t_token_op	op;

	if (ft_strequ(tmp, "eq"))
		op = TOKEN_OP_EQ;
	else if (ft_strequ(tmp, "ge"))
		op = TOKEN_OP_GE;
	else if (ft_strequ(tmp, "gt"))
		op = TOKEN_OP_GT;
	else if (ft_strequ(tmp, "le"))
		op = TOKEN_OP_LE;
	else if (ft_strequ(tmp, "lt"))
		op = TOKEN_OP_LT;
	else if (ft_strequ(tmp, "ne"))
		op = TOKEN_OP_NE;
	else
		op = TOKEN_OP_NONE;
	return (op);
}
