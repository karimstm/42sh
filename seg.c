
#include <unistd.h>
#include <stdio.h>
# include <stdlib.h>
# include <signal.h>

int main()
{
	kill(getpid(), SIGSEGV);
	return (0);
}
