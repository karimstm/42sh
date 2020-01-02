#include "ast.h"
#include "libft.h"

int     open_tmp(char *pathname)
{
    int fd;
    char *path;

    path = ft_strjoin("/tmp/", pathname);
    printf("%s\n", path);
    fd = open(path, O_RDWR, 0644);
    ft_strdel(&path);
    return (fd);
}

int    ft_tmpfile()
{
    int fd;
    int rd;
    char buf[50];
    char *pathname;
    char *tmp;
    
    fd = open("/usr/bin/uuidgen", O_RDWR);
    pathname = NULL;
    if (fd >= 0)
    {
        while ((rd = read(fd, &buf, 20)))
        {
            buf[rd] = 0;
            tmp = pathname;
            pathname = ft_strjoin(pathname, buf);
            ft_strdel(&tmp);
        }
    }
    fd = open_tmp(pathname);
    ft_strdel(&pathname);
    return (fd);
}