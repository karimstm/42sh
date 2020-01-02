/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:58:32 by amoutik           #+#    #+#             */
/*   Updated: 2019/10/12 11:42:55 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.h"
#include "globing.h"

int     main(int argc, char **argv)
{
  t_glob g;
  g.gl_pathc = 0;
  size_t i = 0;

  if (argc == 1)
    return (1);
  _glob(argv[1], GLOB_DOOFFS, &error_func, &g);
 while (i < g.gl_pathc)
 {
	 printf("%s\n", g.gl_pathv[i]);
	 i++;
 } 
  return (0);
}
