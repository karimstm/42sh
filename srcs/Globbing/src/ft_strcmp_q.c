/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp_q.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/01 09:50:09 by amoutik           #+#    #+#             */
/*   Updated: 2019/04/01 13:06:05 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

int   ft_strcmp_q(const char *s1, const char *s2)
{
  char  *start;

  start = (char *)s2;
    while (*s1 && ((*s1 == *s2) || *s2 == QUESTION))
    {
      s1++;
      s2++;
      if (*s2 == '\\' && *(s2 + 1) == STAR)
        s2++;
      if ((char *)s2 > start && *(s2 - 1) != '\\' && *s2 == STAR)
        return (0);
    }
    if (!*s1 && *s2 == SEP)
      return (0);
    return (*(const unsigned char *)s1 - *(const unsigned char *)s2);
}
