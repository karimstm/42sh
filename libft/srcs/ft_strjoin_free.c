/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 08:42:52 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/03/12 10:37:31 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_free(char *s1, char const *s2)
{
	char	*tmp;

	tmp = s1;
	if (s1 == NULL)
		s1 = ft_strdup(s2);
	else
		s1 = ft_strjoin(s1, s2);
	free(tmp);
	return (s1);
}
