/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 02:26:22 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/22 17:16:18 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long count_chars(const char *str, const char *chars, size_t size)
{
    long count;

    count = 0;
    while (size--)
        count += ft_isinstr(*str++, chars);
    return(count);
}

static int join_result(char *dst, char *src, int size)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (i < size)
    {
        if(src[i])
        {
            if (ft_isinstr(src[i], "\"'$(){}\\"))
                dst[j++] = '\\';
            dst[j++] =  src[i];
        }
        i++;
    }
    if (dst[j - 1] == '\n')
        j--;
    dst[j] = 0;
    return(j);
}

char *get_fd_content(int fd)
{
    char buff[100001];
    char *tmp;
    char *ret;
    int r;
    size_t size;

    size = 0;
    ret  = NULL;
    while ((r = read(fd, buff, 100000)) > 0)
    {
        tmp = ret;
        if (!(ret = malloc(size + r + count_chars(buff,  "\"'$(){}\\", r) + 1)))
        {
            free(tmp);
            return(NULL);
        }
        ft_strcpy(ret, tmp ? tmp : "");
        size += join_result(ret + size, buff, r);
        free(tmp);
    }
    return(ret);
}

char *cmd_substitution(const char *cmd)
{
    int fd[2];
    pid_t pid;
    char *ret;

    if (!pipe(fd))
    {
        pid = fork();
        if (pid == 0)
        {
            dup2(fd[1], 1);
            system(cmd);
            close(fd[1]);
            exit(0);
        }
        close(fd[1]);
        if (pid > 0)
        {
            ret = get_fd_content(fd[0]);
            close(fd[0]);
            return (ret);
        }
        close(fd[0]);
    }
    return (NULL);
}
