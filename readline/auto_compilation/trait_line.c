/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trait_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szakaria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 04:13:58 by szakaria          #+#    #+#             */
/*   Updated: 2020/03/01 04:16:06 by szakaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auto_compilation.h"

int			ft_words(char c, int mode)
{
	static int bin;

	if (mode == 1 && (c == '|' || c == '&' || c == ' '))
		return (1);
	else if (mode == 2)
	{
		bin++;
	}
	else if (mode == 3)
		bin = 0;
	if (!mode)
		return (bin);
	return (0);
}

char		*first_words(char *line)
{
	char		*tmp;
	int			i;
	int			j;

	tmp = NULL;
	j = 0;
	i = ft_strlen(line) - 1;
	while (i >= 0)
	{
		if (ft_words(line[i], 1))
		{
			if (line[i] == ' ')
			{
				j = i;
				while (++j >= 0 && (line[j] == ' '))
					if (line[j] == '|' || line[j] == '&')
						ft_words(line[j], 2);
			}
			tmp = ft_strsub(line, (i + 1), ft_strlen(line));
			return (tmp);
		}
		i--;
	}
	return (ft_strdup(line));
}

int			was_aff(char *new_line, char *line_select)
{
	int i;

	if (!new_line || !line_select)
		return (0);
	i = 0;
	while (new_line[i] && line_select[i] && line_select[i] == new_line[i])
		i++;
	return (i);
}

void		send_readline(char **stock, t_readline *env, char *new_line)
{
	int		i;
	char	*line_select;

	i = 0;
	line_select = NULL;
	while (stock[i])
		i++;
	if (i > 1)
	{
		line_select = print_window_element(stock, -2);
		insert_in_line(env, &line_select[was_aff(new_line, line_select)]);
	}
	else if (i == 1)
		insert_in_line(env, &stock[0][was_aff(new_line, stock[0])]);
}

void		auto_compilation(t_readline *env)
{
	char		*new_line;
	char		*tmp2;
	char		**stock;

	stock = NULL;
	new_line = NULL;
	if (!env->cmd->tmp_line)
		return ;
	tmp2 = ft_strsub(env->cmd->tmp_line, 0, env->line_index);
	new_line = first_words(tmp2);
	if (ft_strlen(new_line) && !ft_words('0', 0))
		stock = context_type_var(new_line);
	if (!stock && ft_strlen(new_line) && !ft_words('0', 0))
		stock = context_type_bin(new_line);
	if (!stock)
		stock = context_type_path(new_line);
	if (stock && new_line)
	{
		send_readline(stock, env, new_line);
		free_tab(stock);
	}
	ft_words('0', 3);
	free(tmp2);
	free(new_line);
}
