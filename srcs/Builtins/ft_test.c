/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_test.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 11:28:20 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/18 17:26:11 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		count_argv(char **argv)
{
	size_t		counter;

	counter = 0;
	while (argv[counter])
		counter++;
	return (counter);
}

int		file_flag(int c)
{
	const int flag[256] = {
		['b'] = S_IFBLK,
		['c'] = S_IFCHR,
		['d'] = S_IFDIR,
		['e'] = 'e',
		['f'] = S_IFREG,
		['g'] = S_ISGID,
		['h'] = S_IFLNK,
		['L'] = S_IFLNK,
		['p'] = S_IFIFO,
		['r'] = S_IRUSR,
		['s'] = 's',
		['S'] = S_IFSOCK,
		['u'] = S_ISUID,
		['w'] = S_IWUSR,
		['x'] = S_IXUSR,
		['n'] = 'n',
		['z'] = 'z',
	};

	return (flag[c]);
}

int		unary_error(char *s)
{
	ft_printf_fd(2, "42sh: test: %s: unary operator expected\n", s);
	return (-1);
}

int		file_checker(char **argv)
{
	int			flag;
	char		*tmp;

	flag = 0;
	tmp = argv[0];
	if (ft_strlen(tmp) > 2)
		unary_error(tmp);
	else if (*tmp == '-')
	{
		tmp++;
		if (!(flag = file_flag(*tmp)))
			unary_error(argv[0]);
		else
			return (do_file_checking(argv[1], flag));
	}
	return (-1);
}

int		ft_test(char **argv)
{
	size_t	counter;
	char	reverse;

	reverse = 0;
	counter = 0;
	DECLARE(int, _(status, 0));
	if (ft_strequ(argv[0], "!"))
	{
		reverse = '!';
		argv = &argv[1];
	}
	if ((counter = count_argv(argv)) > 3)
	{
		ft_printf_fd(2, "42sh: test: too many arguments\n");
		return (1);
	}
	if (counter == 1)
		return (0);
	if (counter == 2)
		status = file_checker(argv);
	else if (counter == 3)
		status = expression(argv);
	if (reverse)
		status = !status;
	return (status == -1 ? 2 : !status);
}
