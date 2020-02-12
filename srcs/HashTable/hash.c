#include "shell.h"

static t_hash_table *ht_new_sized(const int base_size)
{
	t_hash_table *ht = (t_hash_table *)xmalloc(sizeof(t_hash_table));
	ht->base_size = base_size;
	ht->size = next_prime(ht->base_size);
	ht->count = 0;
	ht->items = calloc((size_t)ht->size, sizeof(t_hash_item *));
	return (ht);
}

t_hash_table *ht_new()
{
	return ht_new_sized(HT_INITIAL_BASE_SIZE);
}

static void ht_resize(t_hash_table *ht, const int base_size)
{
	t_hash_table    *new_ht;
	t_hash_item     *item;
	t_hash_item     **tmp_items;
	int tmp_size;
	int i;
	if (base_size < HT_INITIAL_BASE_SIZE)
		return ;
	new_ht = ht_new_sized(base_size);
	i = 0;
	while (i < ht->size)
	{
		item = ht->items[i];
		if (item != NULL && item != &HT_DELETED_ITEM)
			ht_insert(new_ht, item->key, item->value);
		i++;
	}
	ht->base_size = new_ht->base_size;
	ht->count = new_ht->count;

	// To delete new_ht, we give it ht's size and items;
	tmp_size = ht->size;
	ht->size = new_ht->size;
	new_ht->size = tmp_size;
	tmp_items = ht->items;
	ht->items = new_ht->items;
	new_ht->items = tmp_items;
	ht_del_hash_table(new_ht);
}

static void ht_resize_up(t_hash_table *ht)
{
	const int new_size = ht->base_size * 2;
	ht_resize(ht, new_size);
}

static void ht_resize_down(t_hash_table *ht)
{
	const int new_size = ht->base_size / 2;
	ht_resize(ht, new_size);
}

static t_hash_item *ht_new_item(const char *k, const char *v)
{
	t_hash_item *item;

	item = (t_hash_item *)xmalloc(sizeof(t_hash_item));
	item->key = strdup(k);
	item->value = strdup(v);
	return (item);
}

static void ht_del_item(t_hash_item *item)
{
	free(item->key);
	free(item->value);
	free(item);
};

void    ht_del_hash_table(t_hash_table *ht)
{
	int i;
	t_hash_item *item;

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

static int hash(const char *s, const int a, const int m)
{
	long        hash;
	int         i;
	const int   len_s = strlen(s);

	hash = 0;
	i = 0;
	while (i < len_s)
	{
		hash += (long)pow(a, len_s - (i + 1)) * s[i];
		hash = hash % m;
		i++;
	}
	return (int)hash;
}

static int ht_get_hash(const char *s, const int num_buckets, const int attempt)
{
	const int hash_a = hash(s, HT_PRIME_1, num_buckets);
	const int hash_b = hash(s, HT_PRIME_2, num_buckets);
	return ((hash_a + (attempt * (hash_b + 1))) % num_buckets);
}

void    ht_insert(t_hash_table *ht, const char *key, const char *value)
{
	int         index;
	t_hash_item *item;
	int          i;
	t_hash_item *current_item;
	const int load = ht->count * 100 / ht->size;

	if (load > 70)
		ht_resize_up(ht);
	item = ht_new_item(key, value);
	index = ht_get_hash(item->key, ht->size, 0);
	current_item = ht->items[index];
	i = 1;
	while (current_item != NULL)
	{
		if (current_item != &HT_DELETED_ITEM)
		{
			if (strcmp(current_item->key, key) == 0)
			{
				ht_del_item(current_item);
				ht->items[index] = item;
				return ;
			}
		}
		index = ht_get_hash(item->key, ht->size, i);
		current_item = ht->items[index];
		i++;
	}
	ht->items[index] = item;
	ht->count++;
}


char    *ht_search(t_hash_table *ht, const char *key)
{
	int index;
	t_hash_item *item;
	int i;
	index = ht_get_hash(key, ht->size, 0);
	item = ht->items[index];
	i = 1;
	while (item != NULL)
	{
		if (item != &HT_DELETED_ITEM)
		{
			if (strcmp(item->key, key) == 0)
				return(ft_strdup(item->value)); // We will strdup tomporarly here untile we do something
		}										// about it in the execution process to avoid double free
		index = ht_get_hash(key, ht->size, i);
		item = ht->items[index];
		i++;
	}
	return (NULL);
}

void    ht_delete(t_hash_table *ht, const char *key)
{
	int index;
	t_hash_item *item;
	int i;
	const int load = ht->count * 100 / ht->size;

	if (load < 10) {
		ht_resize_down(ht);
	}
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

