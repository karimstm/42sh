/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:12:46 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 16:13:29 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			get_size(t_stack *sp)
{
	return (sp->size);
}

int			is_overflow(t_stack *sp)
{
	return (sp->top == sp->size - 1);
}

int			is_underflow(t_stack *sp)
{
	return (sp->top == -1);
}
