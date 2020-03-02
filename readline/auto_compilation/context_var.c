/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 04:21:03 by szakaria          #+#    #+#             */
/*   Updated: 2020/03/02 06:00:56 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_compilation.h"

char			**context_var(char *line, int i)
{
	t_variables			*head;
	char				**tab;
	char				*tmp;

	tab = (char **)malloc(sizeof(char *) * i + 1);
	tab[i] = NULL;
	tmp = NULL;
	i = 0;
	head = g_env2->head;
	while (head)
	{
		if (!ft_strncmp(head->key, &line[2], ft_strlen(&line[2])))
		{
			tmp = ft_strjoin("${", head->key);
			tab[i++] = ft_strjoin(tmp, "}");
			ft_strdel(&tmp);
		}
		head = head->next;
	}
	return (tab);
}

char			**context_type_var(char *new_line)
{
	t_variables			*head;
	int					cont;

	cont = 0;
	head = g_env2->head;
	if (new_line && new_line[0] == '$' && new_line[1] == '{')
	{
		while (head)
		{
			if (!ft_strncmp(head->key, &new_line[2], ft_strlen(&new_line[2])))
				cont++;
			head = head->next;
		}
		if (cont)
			return (context_var(new_line, cont));
	}
	return (NULL);
}
