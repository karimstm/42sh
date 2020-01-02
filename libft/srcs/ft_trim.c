/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 11:16:42 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/03/12 11:38:40 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		len_without_end_spaces(char *str)
{
	int		count;
	int		len;
	int		i;

	len = ft_strlen(str);
	i = len;
	count = 0;
	while (ft_iswhitespace(str[--i]))
		count++;
	return (len - count);
}

char			*ft_trim(char *src)
{
	char	*new;
	int		len;

	while (ft_iswhitespace(*src))
		src++;
	len = len_without_end_spaces(src);
	new = (char*)ft_memalloc(sizeof(char) * (len + 1));
	ft_strncpy(new, src, len);
	new[len] = '\0';
	return (new);
}
