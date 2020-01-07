#include "ast.h"
#include "libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

static int  my_random()
{
    static int l = 12;
    return (++l);
}

int     open_tmp(char *pathname)
{
    int fd;
    char *path;

    path = ft_strjoin("/tmp/", pathname);
    fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    ft_strdel(&path);
    return (fd);
}

char    *ft_tmpfile()
{
    int fd;
    int buf;
    char *pathname;

    fd = open("/dev/random", O_RDONLY);
    buf = my_random();
    pathname = NULL;
    if (fd >= 0)
    {
        read(fd, &buf, sizeof(int));
        close(fd);
    }
    if (buf < 0)
        buf *= -1;
    pathname = ft_itoa(buf);
    return (pathname);
}
