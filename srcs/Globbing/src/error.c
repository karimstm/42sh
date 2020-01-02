/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:59:47 by amoutik           #+#    #+#             */
/*   Updated: 2019/04/04 14:00:51 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

int     error_func(const char *epath, int errno)
{
  fprintf(stderr, "error on file : %s\n", epath);
  return (errno);
}
