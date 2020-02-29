/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 17:51:39 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/29 12:49:17 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_hash_item		*ht_new_item(const char *k, const char *v)
{
	t_hash_item *item;

	item = (t_hash_item *)xmalloc(sizeof(t_hash_item));
	item->key = strdup(k);
	item->value = strdup(v);
	return (item);
}

void			ht_del_item(t_hash_item *item)
{
	ft_strdel(&item->key);
	ft_strdel(&item->value);
	free(item);
}

int				hash(const char *s, const int a, const int m)
{
	long		hash;
	int			i;
	const int	len_s = ft_strlen(s);

	hash = 0;
	i = 0;
	while (i < len_s)
	{
		hash += (long)ft_pow(a, len_s - (i + 1)) * s[i];
		hash = hash % m;
		i++;
	}
	return (int)hash;
}

t_hash_table	*ht_new_sized(const int base_size)
{
	t_hash_table *ht;

	ht = (t_hash_table *)xmalloc(sizeof(t_hash_table));
	ht->base_size = base_size;
	ht->size = next_prime(ht->base_size);
	ht->count = 0;
	ht->items = (t_hash_item **)ft_memalloc(sizeof(t_hash_item *)
					* (size_t)ht->size);
	return (ht);
}

t_hash_table	*ht_new(void)
{
	return (ht_new_sized(HT_INITIAL_BASE_SIZE));
}
