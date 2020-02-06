/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/03 16:16:54 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/05 17:05:48 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_alias_list		*get_alias_list(t_alias_list *aliases)
{
	static t_alias_list	*list = NULL;

	if (aliases != NULL)
		list = aliases;
	return (list);
}

void			init_alias(t_alias_list *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->node_count = 0;
}

void			alias_push(t_alias_list *list, char *alias, char *value)
{
	t_alias	*new_alias;

	new_alias = (t_alias *)malloc(sizeof(t_alias));
	new_alias->alias = alias;
	new_alias->value = value;
	new_alias->alias_len = ft_strlen(alias);
	new_alias->next = NULL;
	if (list->node_count == 0 || list->head == NULL)
		list->head = new_alias;
	else
		list->tail->next = new_alias;
	list->tail = new_alias;
	list->node_count++;
}

int				is_alias_pattern(char *str)
{
	char *start;

	start = str;
	while (*start)
	{
		if (*start == '=')
			return (start - str);
		start++;
	}
	return (0);
}

char			**split_eq(char *str)
{
	char		**list;
	const int	len = is_alias_pattern(str);

	list = (char **)malloc(sizeof(char *) * 3);
	list[0] = ft_strndup(str, len);
	list[1] = ft_strdup(&str[len + 1]);
	list[2] = NULL;
	return (list);
}

/*
** Function responsible for inserting a new alias to list
*/

void			alias_insert(t_alias_list *list, char *args)
{
	size_t			len;
	t_alias			*current;
	char			**alias;
	char			*tmp;

	current = NULL;
	if (list && list->head)
		current = list->head;
	alias = split_eq(args);
	len = ft_strlen(alias[0]);
	while (current)
	{
		if (len == current->alias_len && strcmp(alias[0], current->alias) == 0)
		{
			tmp = current->value;
			current->value = alias[1];
			ft_strdel(&tmp);
			free(alias[0]);
			free(alias);
			return ;
		}
		current = current->next;
	}
	alias_push(list, alias[0], alias[1]);
	free(alias);
}

t_alias			*alias_find(t_alias_list *list, char *alias)
{
	t_alias			*current;
	const size_t	len = ft_strlen(alias);

	current = NULL;
	if (list && list->head)
		current = list->head;
	while (current)
	{
		if (len == current->alias_len && strcmp(alias, current->alias) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void			aliase_prune(t_alias_list *list)
{
	t_alias *current;
	t_alias	*tmp;

	current = NULL;
	if (list && list->head)
		current = list->head;
	while (current)
	{
		tmp = current->next;
		free(current->alias);
		free(current->value);
		free(current);
		current = NULL;
		current = tmp;
	}
	init_alias(list);
	free(list);
	list = NULL;
}

char			*get_alias_value(t_alias_list *list,
								char *alias, char *start_alias)
{
	t_alias			*current;
	t_alias			*tmp_alias;
	char			*value;
	size_t			len;
	t_alias_list	*tmp_list;

	current = NULL;
	value = NULL;
	current = (list && list->head) ? list->head : NULL;
	len = ft_strlen(alias);
	init_alias((tmp_list = (t_alias_list *)malloc(sizeof(t_alias_list))));
	while (current)
	{
		if (len == current->alias_len && strcmp(alias, current->alias) == 0)
		{
			if ((tmp_alias = alias_find(tmp_list, alias)))
			{
				if (strcmp(start_alias, alias) == 0)
					value = start_alias;
				value = tmp_alias->value;
				break ;
			}
			value = current->value;
			alias_push(tmp_list, ft_strdup(alias), ft_strdup(value));
			alias = value;
			len = ft_strlen(alias);
			current = list->head;
		}
		current = current->next;
	}
	aliase_prune(tmp_list);
	return (value);
}

char			*get_alias(char *key)
{
	t_alias_list *list;
	char		 *value;

	list = get_alias_list(NULL);
	value = get_alias_value(list, key, key);
	return (value);
}

void			delete_alias(t_alias_list *list, char *to_find)
{
	t_alias			*current;
	t_alias			*next;
	t_alias			*prev;
	const size_t	len = ft_strlen(to_find);

	prev = NULL;
	current = (list && list->head) ? list->head : NULL;
	while (current)
	{
		if (len == current->alias_len && strcmp(to_find, current->alias) == 0)
		{
			next = current->next;
			if (current == list->head)
				list->head = next;
			if (current == list->tail)
				list->tail = prev;
			free(current->alias);
			free(current->value);
			free(current);
			list->node_count--;
			current = next;
		}
		prev = current;
		current = current->next;
	}
}

void			print_aliases(t_alias_list *list)
{
	t_alias *current;

	current = NULL;
	if (list && list->head)
		current = list->head;
	while (current)
	{
		printf("alias: %s='%s'\n", current->alias, current->value);
		current = current->next;
	}
}

int			ft_alias(char **args)
{
	int				i;
	t_alias_list	*alias_list;
	t_alias			*alias;
	int				error;

	alias_list = get_alias_list(NULL);
	i = 0;
	error = 0;
	if (args == NULL || !*args)
		print_aliases(alias_list);
	else
	{
		while (args[i])
		{
			if (is_alias_pattern(args[i]))
				alias_insert(alias_list, args[i]);
			else
			{
				if (!(alias = alias_find(alias_list, args[i])))
				{
					ft_printf_fd(2, "42sh: alias: %s: not found\n", args[i]);
					error = 1;
				}
				else
					ft_printf_fd(2, "alias: %s='%s'\n", alias->alias, alias->value);
			}
			i++;
		}
	}
	return (error);
}

int				print_usage(void)
{
	dprintf(2, "unalias: usage: unalias [-a] name [name ...]\n");
	return (2);
}

int				print_invalid_option(char c)
{
	dprintf(2, "42sh: unalias: -%c: invalid option\n", c);
	return (print_usage());
}

int				check_option(char **args)
{
	int		found;
	int		i;
	char	*str;

	found = 1;
	i = 0;
	while (args[i])
	{
		str = args[i++];
		if (str && str[0] == '-')
		{
			str++;
			while (*str)
			{
				if (*str == 'a')
					found = 0;
				else
					return (print_invalid_option(*str));
				str++;
			}
		}
	}
	return (found);
}

int			ft_unalias(char **args)
{
	int				i;
	t_alias_list	*list;

	ft_putendl("TEST");
	list = get_alias_list(NULL);
	i = 0;
	if (args == NULL || *args == NULL)
		return (print_usage());
	else if (list)
	{
		if (!check_option(args))
			aliase_prune(list);
		else
		{
			while (args[i])
			{
				delete_alias(list, args[i]);
				i++;
			}
		}
	}
	return (0);
}
