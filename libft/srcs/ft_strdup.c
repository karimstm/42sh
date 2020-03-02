/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/04 21:29:38 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/03/02 10:37:59 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	int		i;
	int		len;
	char	*str;

	if (src)
	{
		len = ft_strlen(src);
		str = (char *)malloc(sizeof(*src) * (len + 1));
		if (str == ((void *)0))
			return (str);
		i = -1;
		while (src[++i] != '\0')
			str[i] = src[i];
		str[i] = '\0';
		return (str);
	}
	return (NULL);
}
