#include "alias.h"

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
	new_alias->alias_len = strlen(alias);
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
	char	**list;
	const 	int len = is_alias_pattern(str);

	list = (char **)malloc(sizeof(char *) * 3);
	list[0] = strndup(str, len);
	list[1] = strdup(&str[len+1]);
	list[2] = NULL;
	return (list);
}

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
	len = strlen(alias[0]);
	while (current)
	{
		if (len == current->alias_len && strcmp(alias[0], current->alias) == 0)
		{
			tmp = current->value;
			current->value = alias[1];
			free(tmp);
			tmp = NULL; // remember to replace these two line with ft_strdel(&tmp)
			return ;
		}
		current = current->next;
	}
	alias_push(list, alias[0], alias[1]);
	free(alias);
}

void			alias_find(t_alias_list *list, char *alias)
{
	t_alias *current;
	const size_t	len = strlen(alias);

	current = NULL;
	if (list && list->head)
		current = list->head;
	while (current)
	{
		if (len == current->alias_len && strcmp(alias, current->alias) == 0)
		{
			dprintf(2, "alias: %s='%s'\n", current->alias, current->value);
			return ;
		}
		current = current->next;
	}
	dprintf(2, "42sh: alias: %s: not found\n", alias);
}

char			*get_alias_value(t_alias_list *list, char *alias, char *start_alias)
{
	t_alias			*current;
	char			*value;
	char			*tmp;
	size_t			len;

	current = NULL;
	value = NULL;
	if (list && list->head)
		current = list->head;
	len = strlen(alias);
	while (current)
	{
		if (len == current->alias_len && strcmp(alias, current->alias) == 0)
		{
			value = current->value;
			//return (value);
			if (start_alias && strcmp(value, start_alias) == 0)
				return (start_alias);
			if ((tmp = get_alias_value(list, value, start_alias)) == NULL)
				return (value);
			return (tmp);
		}
		current = current->next;
	}
	return (value);
}

void			delete_alias(t_alias_list *list, char *to_find)
{
	t_alias			*current;
	t_alias			*next;
	t_alias			*prev;
	const size_t	len = strlen(to_find);

	current = NULL;
	prev = NULL;
	if (list && list->head)
		current = list->head;
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
			continue ;
		}
		prev = current;
		current = current->next;
	}
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

void			ft_alias(char **args, t_alias_list *alias_list)
{
	int				i;

	i = 0;
	if (args == NULL)
		print_aliases(alias_list);
	else
	{
		while (args[i])
		{
			if (is_alias_pattern(args[i]))
				alias_insert(alias_list, args[i]);
			else
				alias_find(alias_list, args[i]);
			i++;
		}
	}
}

int			print_usage()
{
	dprintf(2, "unalias: usage: unalias [-a] name [name ...]\n");
	return (2);
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
		str = args[i];
		if (str && str[0] == '-')
		{
			str++;
			while (*str)
			{
				if (*str == 'a')
					found = 0;
				else
				{
					dprintf(2, "42sh: unalias: -%c: invalid option\n", *str);
					return print_usage();
				}
				str++;
			}
		}
		i++;
	}
	return (found);
}

void			ft_unalias(char **args, t_alias_list *list)
{
	int			i;

	i = 0;
	if (args == NULL)
		print_usage();
	else
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
}
