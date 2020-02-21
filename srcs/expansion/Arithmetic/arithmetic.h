/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arithmetic.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 03:18:45 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/21 09:40:54 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

#define BETWEEN(v, v1, v2) ((v) >= (v1) && (v) <= (v2))
#define HEX_BASE "0123456789abcdef"
#define OCTAL_BASE "01234567"

long long	do_op(long long *op1, int oparator, long op2, int ret);
int			do_logical_op(t_list **token, long long *result, int operator);
int			get_precedence(int operator);
int			operator_have_precedence(t_list **token, int operator1);
void        ar_edit_var(char *key, long long val, int prefix);

long long	convert_operand(t_list **token, long long *result);
int			convert_operator(t_list **token, int *result, int skip_token);
int			eval_expr(t_list **tokens, long long *result);
t_list      *parse_number(char **expression);
t_list      *parse_variable(char **expr);
t_list      *parse_ar_expression(char **expression, t_list *token);