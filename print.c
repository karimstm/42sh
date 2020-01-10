#include <stdio.h>

int 	*num(void)
{
	static int error = 0;
	return (&error);
}

int		main(int argc, char **argv)
{
	int i = 1;
	int *error = num();
	*error = 2;
	printf("%d\n", *num());
	return (0);
}
