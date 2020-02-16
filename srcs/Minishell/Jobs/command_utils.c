/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 12:27:44 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 12:28:38 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_simple_command		*get_command_name(t_list_simple_command *list)
{
	t_simple_command	*current;

	current = list && list->node_count ? list->head : NULL;
	while (current)
	{
		if (current->kind == TOKEN_WORD)
			return (current);
		current = current->next;
	}
	return (NULL);
}

char					*command_path(char *name)
{
	char	*path;

	path = NULL;
	if ((path = ht_search(HASH_TABLE, name)))
		return (ft_strdup(path));
	else if ((path = working_path(name)))
		return (path);
	return (NULL);
}

t_cmd_type				cmd_type(t_process *p)
{
	char				*path;
	char				*name;

	DECLARE(t_simple_command, _(*cmd, NULL));
	if (p->node->kind == NODE_SIMPLE_COMMAND && SIMPLE_CMD(p->node))
		cmd = get_command_name(SIMPLE_CMD(p->node));
	else
		return (0);
	if ((name = cmd && cmd->name ? cmd->name : NULL) == NULL)
		return (1);
	if (ft_strchr(name, '/'))
		return (cmd->type = IS_PATH_CMD);
	else if (ft_lstsearch(BUILT_LIST, name, &check_builtin))
		return (cmd->type = IS_BUILTIN);
	else if ((path = command_path(name)))
	{
		ft_strdel(&cmd->name);
		cmd->name = path;
		return (cmd->type = IS_FOUND);
	}
	else
	{
		p->status = 127;
		return (cmd->type = IS_NOTFOUND);
	}
}
