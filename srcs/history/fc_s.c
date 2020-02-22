/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc_s.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 06:03:16 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/11 06:03:18 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	replace_it(char **command, char *found, char *rep, char *to_rep)
{
	int		len_command;
	int		len_to_rep;

	*found = '\0';
	len_command = ft_strlen(*command);
	len_to_rep = ft_strlen(to_rep);
	*command = ft_strjoin_pre_free(*command, to_rep, found + ft_strlen(rep));
	return (len_command + len_to_rep);
}

static char	*fc_s_replace(char *full_command, char *rep)
{
	char	*found;
	char	*to_rep;
	int		index;

	found = NULL;
	index = 0;
	if (rep[0] == '=' || (to_rep = ft_strchr(rep, '=')) == NULL)
		return (full_command);
	*to_rep = '\0';
	to_rep++;
	while ((found = ft_strstr(full_command + index, rep)) != NULL)
		index = replace_it(&full_command, found, rep, to_rep);
	to_rep--;
	*to_rep = '=';
	return (full_command);
}

static int	fc_exec(char *rep, char *cmd)
{
	char	*full_command;
	int		status;
	char	*command;

	status = 1;
	command = search_history(cmd);
	full_command = NULL;
	if (command != NULL)
		full_command = ft_strdup(search_history(cmd));
	if (rep != NULL)
		full_command = fc_s_replace(full_command, rep);
	if (full_command != NULL)
	{
		ft_printf_fd(2, "%s\n", full_command);
		status = sh_system(full_command, 1);
	}
	else
		ft_printf_fd(2, "fc: no command found\n");
	ft_strdel(&full_command);
	return (status);
}

int			fc_s(char *first, char *last)
{
	char	*rep;
	char	*cmd;

	rep = NULL;
	cmd = NULL;
	if (ft_strchr(first, '=') != NULL)
	{
		rep = first;
		cmd = last;
	}
	else
		cmd = first;
	if (cmd == NULL)
		cmd = search_history("!");
	return (fc_exec(rep, cmd));
}
