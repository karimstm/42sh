#include "shell.h"

/*
type steps
    Builtin
    aliases t_alias_list *alias_list global
    hashtable static t_hash_table* global
    path
    not found
*/
char    *working_path_type(char *cmd)
{
	char        **all_paths;
	char        **tmp;
	char        *full_path;
    t_variables *var;

	var = get_var("PATH");
	all_paths = var ? ft_strsplit(var->value, ':') : NULL;
	tmp = all_paths;
	full_path = NULL;
	while (*tmp)
	{
		full_path = ft_strjoin_pre(*tmp, "/", cmd);
		if (access(full_path, F_OK) == 0)
			break ;
		ft_strdel(&full_path);
		tmp++;
	}
	ft_free_strtab(all_paths);
	return (full_path);
}

char    *is_aliased(char *arg)
{
    t_alias_list    *aliases;
    t_alias         *cur;

    aliases = get_alias_list(NULL);
    cur = aliases->head;
    while (cur)
    {
        if (ft_strequ(arg, cur->alias))
            return (cur->value);
        cur = cur->next;
    }
    return (NULL);
}

int    ft_type(char **args)
{
    t_list  *blt;
    char    *value;
    int     i;
    int     ret;

    blt = get_set_blt(NULL);
    i = 0;
    ret = 1;
    while (args[i])
    {
        ret = 0;
        if (ft_lstsearch(blt, args[i], &check_builtin))
            ft_printf("%s is a shell builtin\n", args[i]);
        else if ((value = is_aliased(args[i])))
            ft_printf("%s is aliased to `%s'\n", args[i], value);
        else if ((value = ht_search(get_hash_table(0), args[i])))
            ft_printf("%s is hashed (%s)\n", args[i], value);
        else if ((value = working_path_type(args[i])))
            ft_printf("%s is %s\n", args[i], value);
        else
            ret = ft_printf("42sh: type: %s: not found\n", args[i]);
        i++;
    }
    return (ret);
}