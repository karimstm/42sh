/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_sub.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:16:23 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/20 16:44:43 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		skip_dqoute(char **string, char c)
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

void		waiting_proc()
{

}

void		setup_proc(char *cmd, char *path, int flag)
{
	pid_t child;
	int		output;
	int		fd;

	output = flag == O_WRONLY ? dup3(1) : dup3(0);
	if ((child = fork()) == 0)
	{
		t_fifo_list *list = (t_fifo_list *)xmalloc(sizeof(t_fifo_list));
		init_fifo_list(list);
		fifo_list(list);
		if ((fd = open(path,  flag)) == -1)
			exit(EXIT_FAILURE);
		flag == O_WRONLY ? dup2(fd, 1) : dup2(fd, 0);
		sh_system(cmd, 0);
		close_fifos(list);
		free(list);
		exit(EXIT_SUCCESS);
	}
	else
	{
		dup2(output, flag == O_WRONLY ? 1 : 0);
		close(output);
	}
}

char		*setup_fifo()
{
	char	*path;

	path = ft_tpname(NULL);
	if (!mkfifo(path, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH))
		return (path);
	return (NULL);
}

char		*scan_pro(char *name)
{
	size_t	len;
	char	*new;

	len = 0;
	new = NULL;
	DECLARE(int, _(index, -1), stack[100]);
	DECLARE(char, _(*start, name));
	while (*name)
	{
		if (*name == '"' || *name == '\'')
			skip_dqoute(&name, *name);
		if (*name == '(')
			stack[++index] = '(';
		else if (*name == ')')
			index--;
		if (index == -1)
			break ;
		name++;
	}
	len = name - start;
	if (index == -1 && len > 1)
		new = ft_strsub(++start, 0, len - 1);
	return (new);
}

/*
** <(ls -la)
** [word]<(ls -la)[word]
** the word can be quoted
** therefore quoting need to be handled
*/

char		*process_sub_parser(char *name)
{
	char		*start;
	char		*proc;
	t_string	string;
	int			status;
	int			fd;

	string.string = NULL;
	new_string(&string);
	while (*name)
	{
		if ((*name == '<' && *(name + 1) == '(' && (status = '<'))
			|| (*name == '>' && *(name + 1) == '(' && (status = '>')))
		{
			proc = scan_pro(name + 1);
			name += 2;
			char *path = setup_fifo();
			setup_proc(proc, path, status == '<' ? O_WRONLY : O_RDONLY);
			fd = open(path,  status == '<' ? O_RDONLY : O_WRONLY);
			ft_strdel(&proc);
			proc = dev_path(fd);
			fifo_push(FIFO_LIST, path, fd);
			start = proc;
			while (*proc)
				push(&string, *proc++);
			ft_strdel(&start);
			break ;
		}
		else
			push(&string, *name);
		if (*name)
			name++;
	}
	return (string.string);
}




void		setup_sub_proce(t_list_simple_command *list)
{
	t_simple_command *current;

	current = list && list->head ? list->head : NULL;

	while (current)
	{
		if (current->kind == TOKEN_WORD)
			current->name = process_sub_parser(current->name);
		current = current->next;
	}
}
