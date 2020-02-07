# ifndef HASH_H
# define HASH_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
# include "math.h"

# define HT_PRIME_1 (131)
# define HT_PRIME_2 (137)
# define HT_INITIAL_BASE_SIZE (50)

typedef struct s_hash_item {
    char *key;
    char *value;
}               t_hash_item;

typedef struct s_hash_table
{
    int         size;
    int         base_size;
    int         count;
    t_hash_item **items;
}               t_hash_table;


t_hash_table        *ht_new();

static              t_hash_item HT_DELETED_ITEM  = {NULL, NULL};

int                 ft_handle_hash_table(char **args);
void                ht_del_hash_table(t_hash_table *ht);
void                ht_insert(t_hash_table *ht, const char *key, const char *value);
char                *ht_search(t_hash_table *ht, const char *key);
void                ht_delete(t_hash_table *h, const char *key);
int                 is_prime(const int x);
int                 next_prime(int x);
t_hash_table        *get_hash_table(t_hash_table *htab);
# endif