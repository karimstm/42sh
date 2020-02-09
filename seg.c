
#include <unistd.h>
#include <stdio.h>
# include <stdlib.h>
# include <signal.h>
#include <string.h>
#include <unistd.h>
#define name(some) some##test

int main(int ac, char **argv)
{
	kill(getpid(), SIGABRT);
}