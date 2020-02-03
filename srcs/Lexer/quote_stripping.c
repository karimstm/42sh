#include "shell.h"

char		*quote_stripping(char *str)
{
	t_string	string;
	char		*tmp;

	string.string = NULL;
	new_string(&string);
	tmp = str;
	while (*tmp)
	{
		if (*tmp == '\\')
			tmp++;
		else if (*tmp == '"' || *tmp == '\'')
			tmp++;
		else if (*tmp)
			push(&string, *tmp++);
	}
	return (string.string);
}
