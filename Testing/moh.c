#include <stdio.h>
#define DECLARE(x, type) type x
#define NAME(x, y) x##y
#define DEC(x) x
#define _(a, b) a = b
#define ULTIMATE_DECLARATION(type, ...)  type __VA_ARGS__

int		main(void)
{
	ULTIMATE_DECLARATION(int, a = 1, _(b, 5), _(c, 8), _(d, 9), _(e, 8));
	printf("%d %d %d\n", a, b, c);
	return (0);
}
