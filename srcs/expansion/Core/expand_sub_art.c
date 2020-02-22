/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_sub_art.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 20:00:49 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/22 14:51:00 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

t_parser_expansion expand_sub_art(char *str)
{
	t_parser_expansion ret;
	char *tmp[2];
	int is_arithmetic;

	ft_bzero(&ret, sizeof(t_parser_expansion));
	tmp[1] = NULL;
	is_arithmetic = 0;
	if (!(tmp[0] = get_matched_bracket(str, "(){}''\"\"", NULL, 1)))
		return (ret);
	ft_printf("galt lik mayamkanch");
	if (str[1] == '(')
	{
		tmp[1] = get_matched_bracket(str + 1, "(){}''\"\"", NULL,  1);
		is_arithmetic = tmp[0] - tmp[1] == 1;
	}
	ret.index = tmp[0] + 1;
	tmp[0][0] = 0;
	tmp[1] ? (tmp[1][0] = 0) : 0;
	ft_printf("ha result: %s\n",str + 1);
	if (!is_arithmetic)
		 ret.str = cmd_substitution(str + 1);
	else if ((tmp[0] = expand(ft_strdup(str + 2), NULL)))
	{
		ret.str = expand_ar_expr(tmp[0]);
		free(tmp[0]);
	}
	return (ret);
}