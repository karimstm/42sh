/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 09:42:19 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/05 10:36:52 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_H
# define QUOTE_H
# include "libft.h"
# define EOS '\0'
# define SH_TOKEN "<>&|;"


typedef struct			s_string
{
	char				*string;
	size_t				cap;
	size_t				len;
}						t_string;


/*
** string.c
*/

void					new_string(t_string *str);
void					push(t_string *str, char c);
char					space(char c);


/*
** free_string.c
*/
void					free_string(t_string *str);

/*
** is_number.c
*/

int						is_number(char *str);

#endif
