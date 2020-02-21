/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_chars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 21:33:17 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/20 21:42:52 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long ft_count_chars(const char *str, const char *chars)
{
    long count;

    count = 0;
    while (*str)
        count += ft_isinstr(*str, chars);
    return(count);
}