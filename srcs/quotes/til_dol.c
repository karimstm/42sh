/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   til_dol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 12:42:14 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/04 18:33:59 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		handle_dollar(char **ptr, t_string *str)
{
	char	*head;
	char	*tmp;
	char	*env;
	t_list	*env_list;

	head = *ptr;
	env_list = get_t_line()->env;
	if (!ft_isalnum(*(head + 1)))
		return (0);
	else
		head++;
	while (*head && ft_isalnum(*head))
		head++;
	tmp = ft_strndup(*ptr, head - *ptr);
	if ((env = "something") == NULL)
		env = "";
	free(tmp);
	while (*env)
		push(str, *env++);
	*ptr = head;
	return (1);
}

int		handle_tilda(char **ptr, t_string *str)
{
	char	*head;
	char	*home;
	int		j;
	t_list	*env_list;

	j = 0;
	head = *ptr;
	env_list = get_t_line()->env;
	if ((ft_isprint(*(head + 1)) && *(head + 1) != '/') || str->len > 0)
		return (0);
	if ((home = "something") == NULL)
		home = "";
	while (home[j])
		push(str, home[j++]);
	*ptr = head + 1;
	return (1);
}
