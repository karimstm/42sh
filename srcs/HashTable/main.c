#include "hash.h"
#include <stdio.h>

char *get_path(char *key)
{
    char *str = malloc(sizeof(char) * 80);
    // this function meant to mimic the functionality of get_path in real shell app
    // in our case we will return a dummy text;
    strcat(str, "/mybin/");
    strcat(str, key);
    return (str);
}

void    ht_print(t_hash_table *ht)
{
    int i = 0;
    while (i < ht->size)
    {
        if (ht->items[i] != NULL)
            printf("%s=%s\n", ht->items[i]->key, ht->items[i]->value);
        i++;
    }
}

void            lookup_and_insert(t_hash_table *ht, char **args)
{
    int     i;
    char    *str;

    i = 0;
    while (args[i])
    {
        if ((str = get_path(args[i])))
            ht_insert(ht, args[i], str);
        else
            printf("42sh: hash: %s: not found\n", args[i]);
        i++;
    }
}

t_hash_table    *handle_hash_table(t_hash_table *ht, char **args)
{
    t_hash_table    *new_table;
    t_hash_table    *tmp;
    char            *str;

    new_table = NULL;
    if (args)
    {
        if (args[0][0] == '-' && args[0][1] == 'r')
        {
            new_table = ht_new();
            lookup_and_insert(new_table, &args[1]);
        } else 
            lookup_and_insert(ht, args);
    } else 
        ht_print(ht);
    if (new_table)
    {
        tmp = ht;
        ht = new_table;
        ht_del_hash_table(tmp);
    }
    return (ht);
}




int main()
{
    t_hash_table *ht = ht_new();
    char *args[] = { 
        "-r",
        "vim",
        "ls",
        NULL
    };
    ht_insert(ht, "ls", "/bin/ls");
    ht_insert(ht, "echo", "/bin/echo");
    ht_insert(ht, "emacs", "/usr/bin/emacs");
    ht_insert(ht, "vim", "usr/bin/vim");
    ht_insert(ht, "grep", "usr/bin/grep");
    ht = handle_hash_table(ht, args);
    ht = handle_hash_table(ht, NULL);
    // ht_print(ht);
    ht_del_hash_table(ht);
}
