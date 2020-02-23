/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 02:26:22 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/22 17:10:43 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

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
        buff[r] = 0;
        size += r;
        tmp = ret;
        if (!(ret = malloc(size + 1)))
        {
            free(tmp);
            return(NULL);
        }
        bzero(ret, size + 1);
        strcpy(ret, tmp ? tmp : "");
        strcat(ret + (size - r), buff);
        free(tmp);
    }
    return(ret);
}

// char *get_fd_content(int fd)
// {
//     char buff[100001];
//     char *tmp;
//     char *ret;
//     int r;
//     size_t size;

//     size = 0;
//     ret = ft_strdup("");
//     while ((r = read(fd, buff, 100000)) > 0)
//     {
//         buff[r] = 0;
//         size += r;
//         tmp = ret;
//         if (!(ret = ft_strnjoin((char *[]){ret, buff}, 2)))
//         {
//             free(tmp);
//             return (NULL);
//         }
//         free(tmp);
//     }
//     return (ret);
// }

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

// int main(int ac, char **av)
// {
//     (void)ac;
//     printf("%s",cmd_substitution(av[1]));
// }
