/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:59:47 by amoutik           #+#    #+#             */
/*   Updated: 2020/03/02 14:29:48 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

int		error_func(const char *epath, int errno)
{
	ft_printf_fd(STDERR_FILENO, "error on file : %s\n", epath);
	return (errno);
}

char	*ft_get_starting_path(char *path)
{
	size_t	len;

	len = ft_strlen(path);
	while (len)
	{
		if (path[len] == SEP)
			return (ft_strndup(path, len));
		len--;
	}
	return (ft_strdup("."));
}

char	*ft_basename(char *path)
{
	size_t len;

	len = ft_strlen(path);
	while (len--)
	{
		if (path[len] == SEP)
			return (ft_strdup(&path[++len]));
	}
	return (ft_strdup(path));
}

int		skip_dquote(char **string, char c)
{
	char	*new;
	size_t	len;

	new = *string;
	new++;
	while (*new)
	{
		if (*new == '\\' && c != '\'')
			new++;
		else if (*new == c)
		{
			new++;
			break ;
		}
		*new ? new++ : 0;
	}
	len = new - *string;
	*string = new;
	return (len);
}

void	ft_glob_loop(char *pattern, t_glob *pglob)
{
	char		*dirname;
	int			index;
	int			len;
	char		*start;

	dirname = ft_memalloc(MAXPATHLEN);
	index = 0;
	while (*pattern)
	{
		if (*pattern == '\\')
			pattern++;
		else if (*pattern == '"' || *pattern == '\'')
		{
			start = pattern;
			len = skip_dquote(&pattern, *pattern);
			ft_strncpy(&dirname[index], start, len);
			index += len;
		}
		else if (*pattern == '*' || *pattern == '?' || *pattern == '[')
			return (exec_glob(dirname, pattern, pglob));
		dirname[index++] = *pattern;
		if (*pattern)
			pattern++;
	}
	ft_strdel(&dirname);
}
