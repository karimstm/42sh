#include <stdio.h>
#define DECLARE(x, type) type x
#define NAME(x, y) x##y
#define dec(x) x //NOT NORM VALID
#define ULTIMATE_DECLARATION(...) __VA_ARGS__ //NOT NORM VALID

int		main(void)
{
	DECLARE(a, int);
	a = 4;
	printf("%d\n", a);
	return (0);
}
