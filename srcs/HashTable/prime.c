/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prime.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 18:23:35 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 18:23:50 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			is_prime(const int x)
{
	int i;

	i = 3;
	if (x < 2)
		return (-1);
	if (x < 4)
		return (1);
	if ((x % 2) == 0)
		return (0);
	while (i <= floor(sqrt((double)x)))
	{
		if ((x % i) == 0)
			return (0);
		i += 2;
	}
	return (1);
}

/*
** Return the next prime after x, or x if x is prime
*/

int			next_prime(int x)
{
	while (is_prime(x) != 1)
		x++;
	return (x);
}
