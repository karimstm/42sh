/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 12:36:31 by cjamal            #+#    #+#             */
/*   Updated: 2020/02/29 12:41:46 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int						ht_get_hash(const char *s,
				const int num_buckets, const int attempt)
{
	const int hash_a = hash(s, HT_PRIME_1, num_buckets);
	const int hash_b = hash(s, HT_PRIME_2, num_buckets);

	return ((hash_a + (attempt * (hash_b + 1))) % num_buckets);
}

void					ht_del_hash_table(t_hash_table *ht)
{
	int			i;
	t_hash_item	*item;

	i = 0;
	while (i < ht->size)
	{
		item = ht->items[i];
		if (item != NULL)
			ht_del_item(item);
		i++;
	}
	free(ht->items);
	free(ht);
}

void					ht_insert(t_hash_table *ht,
							const char *key, const char *value)
{
	int				index;
	t_hash_item		*current_item;
	const int		load = ht->count * 100 / ht->size;

	if (load > 70)
		ht_resize_up(ht);
	DECLARE(t_hash_item, _(*item, ht_new_item(key, value)));
	DECLARE(int, _(i, 0));
	index = ht_get_hash(item->key, ht->size, 0);
	current_item = ht->items[index];
	while (current_item != NULL)
	{
		if (current_item != &HT_DELETED_ITEM)
			if (strcmp(current_item->key, key) == 0)
			{
				ht_del_item(current_item);
				ht->items[index] = item;
				return ;
			}
		index = ht_get_hash(item->key, ht->size, i);
		current_item = ht->items[index];
		i++;
	}
	ht->items[index] = item;
	ht->count++;
}

char					*ht_search(t_hash_table *ht, const char *key)
{
	int			index;
	t_hash_item	*item;
	int			i;

	index = ht_get_hash(key, ht->size, 0);
	item = ht->items[index];
	i = 1;
	while (item != NULL)
	{
		if (item != &HT_DELETED_ITEM)
		{
			if (strcmp(item->key, key) == 0)
				return (item->value);
		}
		index = ht_get_hash(key, ht->size, i);
		item = ht->items[index];
		i++;
	}
	return (NULL);
}

void					ht_delete(t_hash_table *ht, const char *key)
{
	int				index;
	t_hash_item		*item;
	int				i;
	const int		load = ht->count * 100 / ht->size;

	if (load < 10)
		ht_resize_down(ht);
	index = ht_get_hash(key, ht->size, 0);
	item = ht->items[index];
	i = 1;
	while (item != NULL)
	{
		if (item != &HT_DELETED_ITEM)
		{
			if (strcmp(item->key, key) == 0)
			{
				ht_del_item(item);
				ht->items[index] = &HT_DELETED_ITEM;
			}
		}
		index = ht_get_hash(key, ht->size, i);
		item = ht->items[index];
		i++;
	}
	ht->count--;
}
