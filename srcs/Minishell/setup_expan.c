/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_expan.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/21 20:15:41 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/21 20:16:34 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		setup_expan(t_list_simple_command *list)
{
	setup_sub_proce(list);
	expand_args(list);
	init_expansion(list);
}
