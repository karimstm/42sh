/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/02 11:26:41 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/18 15:37:08 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_list		*get_set_blt(t_list	*blt)
{
	static t_list *list = NULL;

	if (blt != NULL)
		list = blt;
	return (list);
}

/*
**	called from init_builtin,
**	it create a new element of struct t_builtin and fill it,
**	and call ft_lstadd libft function to add this element to the list
*/

static void	add_builtin(t_list **lst, char *cmd, int (*f)())
{
	t_builtin	elem;

	elem.cmd = ft_strdup(cmd);;
	elem.f = f;
	ft_lstadd(lst, ft_lstnew(&elem, sizeof(t_builtin)));
	get_set_blt(*lst);
}

/*
**	function that initiates the builtins commands
**	it adds struct t_builtin elements to the list by calling add_builtin
**	struct t_builtin have a name and a function check minishell.h
*/

void		init_builtin(t_list **lst)
{
	add_builtin(lst, "echo", &ft_echo);
	add_builtin(lst, "cd", &ft_cd);
	add_builtin(lst, "env", &ft_env);
	add_builtin(lst, "pwd", &ft_pwd);
	add_builtin(lst, "set", &ft_set);
	add_builtin(lst, "unset", &ft_unset);
	add_builtin(lst, "jobs", &ft_job);
	add_builtin(lst, "exit", &ft_exit);
	add_builtin(lst, "fg", &ft_fg);
	add_builtin(lst, "bg", &ft_bg);
	add_builtin(lst, "export", &ft_export);
	add_builtin(lst, "alias", &ft_alias);
	add_builtin(lst, "unalias", &ft_unalias);
	add_builtin(lst, "type", &ft_type);
	add_builtin(lst, "hash", &ft_handle_hash_table);
	add_builtin(lst, "history", &ft_history);
	add_builtin(lst, "fc", &ft_fc);
	add_builtin(lst, "test", &ft_test);
}

/*
**	this function needed by ft_lstsearch libft function
**	that called from minishell loop function
**	to check if the command is a builtin function or not
*/

int			check_builtin(t_list *elem, void *obj)
{
	if (ft_strcmp(((t_builtin*)elem->content)->cmd, (char*)obj) == 0)
		return (1);
	else
		return (0);
}

/*
**	it frees all memory allocated in the heap created by add_builtin
**	and frees the t_list element.
*/

void		free_builtin(t_list *lst)
{
	t_list		*previous;

	while (lst)
	{
		free(((t_builtin*)lst->content)->cmd);
		free((t_builtin*)lst->content);
		previous = lst;
		lst = lst->next;
		free(previous);
	}
}
