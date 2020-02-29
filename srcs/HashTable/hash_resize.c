/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_resize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 12:38:32 by cjamal            #+#    #+#             */
/*   Updated: 2020/02/29 12:47:52 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void			ht_resize(t_hash_table *ht, const int base_size)
{
	t_hash_item		*item;
	t_hash_item		**tmp_items;
	int				tmp_size;

	if (base_size < HT_INITIAL_BASE_SIZE)
		return ;
	DECLARE(t_hash_table, _(*new_ht, ht_new_sized(base_size)));
	DECLARE(int, _(i, 0));
	while (i < ht->size)
	{
		item = ht->items[i];
		if (item != NULL && item != &HT_DELETED_ITEM)
			ht_insert(new_ht, item->key, item->value);
		i++;
	}
	ht->base_size = new_ht->base_size;
	ht->count = new_ht->count;
	tmp_size = ht->size;
	ht->size = new_ht->size;
	new_ht->size = tmp_size;
	tmp_items = ht->items;
	ht->items = new_ht->items;
	new_ht->items = tmp_items;
	ht_del_hash_table(new_ht);
}

void				ht_resize_up(t_hash_table *ht)
{
	const int new_size = ht->base_size * 2;

	ht_resize(ht, new_size);
}

void				ht_resize_down(t_hash_table *ht)
{
	const int new_size = ht->base_size / 2;

	ht_resize(ht, new_size);
}
