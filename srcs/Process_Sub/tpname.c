/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tpname.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 11:05:57 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/21 15:41:30 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char		*ft_tpname(const char *tmpdir)
{
	char		*newfile;
	char		*path;

	if (tmpdir == NULL)
		tmpdir = "/tmp/";
	newfile = ft_tmpfile();
	path = ft_strjoin(tmpdir, newfile);
	ft_strdel(&newfile);
	return (path);
}
