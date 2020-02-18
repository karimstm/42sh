#include "shell.h"

t_hash_table    *get_hash_table(t_hash_table *htab)
{
    static t_hash_table *table = NULL;

    if (htab != NULL)
        table = htab;
    return (table);
}

char			*working_path_hash(char *cmd, t_cmd_type *type)
{
	char		**all_paths;
	char		**tmp;
	t_variables	*var;
	char		*full_path;

    if (ft_lstsearch(get_set_blt(NULL), cmd, &check_builtin) || ft_strchr(cmd, '/'))
    {
        *type = IS_BUILTIN;
        return (NULL);
    }
    var = get_var("PATH");
	all_paths = var ? ft_strsplit(var->value, ':') : NULL;
	tmp = all_paths;
	full_path = NULL;
	while (*tmp)
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
    t_cmd_type    type;

    i = 0;
    while (args[i])
    {

        if ((str = working_path_hash(args[i], &type)))
            ht_insert(ht, args[i], str);
        else if (type == IS_NOTFOUND)
            ft_printf("42sh: hash: %s: not found\n", args[i]);
        i++;
    }
}

int    ft_handle_hash_table(char **args)
{
    t_hash_table    *new_table;
    t_hash_table    *tmp;
    t_hash_table    *ht;

    new_table = NULL;
    ht = get_hash_table(NULL);
    if (args && args[0])
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
    get_hash_table(ht);
    return (0);
}




// int main()
// {
//     t_hash_table *ht = ht_new();
//     char *args[] = {
//         "-r",
//         "vim",
//         "ls",
//         NULL
//     };
//     ht_insert(ht, "ls", "/bin/ls");
//     ht_insert(ht, "echo", "/bin/echo");
//     ht_insert(ht, "emacs", "/usr/bin/emacs");
//     ht_insert(ht, "vim", "usr/bin/vim");
//     ht_insert(ht, "grep", "usr/bin/grep");
//     ht = handle_hash_table(ht, args);
//     ht = handle_hash_table(ht, NULL);
//     // ht_print(ht);
//     ht_del_hash_table(ht);
// }
