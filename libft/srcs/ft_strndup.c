/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 08:32:32 by amoutik           #+#    #+#             */
/*   Updated: 2019/03/17 15:21:35 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strndup(const char *s1, size_t n)
{
	char	*string;
	size_t	len;

	string = NULL;
	if (s1 && (len = ft_strlen(s1)))
	{
		len = len > n ? n : len;
		if ((string = (char *)ft_memalloc(sizeof(char) * len + 1)) == NULL)
			return (NULL);
		ft_strncpy(string, (char *)s1, len);
		string[len] = '\0';
	}
	return (string);
}
