/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 12:25:50 by cjamal            #+#    #+#             */
/*   Updated: 2020/03/02 21:51:59 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_hash_table	*get_hash_table(t_hash_table *htab)
{
	static t_hash_table *table = NULL;

	if (htab != NULL)
		table = htab;
	return (table);
}

char			*working_path_hash(char *cmd, t_cmd_type *type)
{
	char			**all_paths;
	char			**tmp;
	t_variables		*var;

	DECLARE(char, _(*full_path, NULL));
	if (ft_lstsearch(get_set_blt(NULL), cmd, &check_builtin)
			|| ft_strchr(cmd, '/'))
	{
		*type = IS_BUILTIN;
		return (NULL);
	}
	var = get_var("PATH");
	all_paths = var ? ft_strsplit(var->value, ':') : NULL;
	tmp = all_paths;
	while (tmp && *tmp)
	{
		full_path = ft_strjoin_pre(*tmp, "/", cmd);
		if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
			break ;
		ft_strdel(&full_path);
		tmp++;
	}
	ft_free_strtab(all_paths);
	*type = full_path ? IS_FOUND : IS_NOTFOUND;
	return (full_path);
}

void			ht_print(t_hash_table *ht)
{
	int				i;

	i = 0;
	while (i < ht->size)
	{
		if (ht->items[i] != NULL)
			printf("%s=%s\n", ht->items[i]->key, ht->items[i]->value);
		i++;
	}
}

void			lookup_and_insert(t_hash_table *ht, char **args)
{
	int				i;
	char			*str;
	t_cmd_type		type;

	i = 0;
	while (args[i])
	{
		if ((str = working_path_hash(args[i], &type)))
		{
			ht_insert(ht, args[i], str);
			ft_strdel(&str);
		}
		else if (type == IS_NOTFOUND)
			ft_printf("42sh: hash: %s: not found\n", args[i]);
		i++;
	}
}

int				ft_handle_hash_table(char **args)
{
	t_hash_table	*tmp;
	t_hash_table	*ht;

	ht = get_hash_table(NULL);
	DECLARE(t_hash_table, _(*new_table, NULL));
	if (args && args[0])
	{
		if (args[0][0] == '-' && args[0][1] == 'r')
		{
			new_table = ht_new();
			lookup_and_insert(new_table, &args[1]);
		}
		else
			lookup_and_insert(ht, args);
	}
	else
		ht_print(ht);
	if (new_table)
	{
		tmp = ht;
		ht = new_table;
		ht_del_hash_table(tmp);
	}
	get_hash_table(ht);
	return (0);
}
