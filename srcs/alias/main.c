#include "alias.h"


void		test_alias()
{
	char *args[] = {
		"ls=karim",
		"vim=echo",
		"ls=ahmed",
		"ahmed=walid",
		"walid=ls",
		"karim=",
		"a==b",
		"=c",
		"=",
		" ",
		NULL
	};

	char *unalias[] =
	{
		"ls",
		"vim",
		NULL
	};

	char *unalias2[] = {"-ab", NULL};
	char *unalias3[] = {"-a", "ls", NULL};
	t_alias_list *list = (t_alias_list *)malloc(sizeof(t_alias_list));
	init_alias(list);
	ft_alias(args, list);
	printf("=================\n");
	printf("ls value is %s\n", get_alias_value(list, "ls", "ls"));
	ft_alias(NULL, list);
	printf("=================\n");
	ft_unalias(unalias, list);
	printf("=================\n");
	ft_alias(NULL, list);
	printf("=================\n");
	ft_unalias(unalias2, list);
	ft_unalias(unalias3, list);
	printf("=========^^ ========\n");
	ft_alias(NULL, list);
}

int main()
{
	test_alias();
	return (0);
}
