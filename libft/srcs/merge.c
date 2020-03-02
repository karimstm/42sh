/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 18:38:12 by amoutik           #+#    #+#             */
/*   Updated: 2020/03/02 14:25:12 by cjamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

t_path		*merge(t_path *first, t_path *second)
{
	t_path		*dummy;
	t_path		*current;
	t_path		*tmp;

	dummy = (t_path *)malloc(sizeof(t_path));
	current = dummy;
	while (first != NULL && second != NULL)
	{
		if (ft_strcmp(first->path, second->path) <= 0)
		{
			current->next = first;
			first = first->next;
		}
		else
		{
			current->next = second;
			second = second->next;
		}
		current = current->next;
	}
	tmp = dummy->next;
	free(dummy);
	current->next = (first == NULL) ? second : first;
	return (tmp);
}

t_path		*get_mid(t_path *head)
{
	t_path	*fast;
	t_path	*slow;

	if (head == NULL)
		return (head);
	fast = head;
	slow = head;
	while (fast->next != NULL && fast->next->next != NULL)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	return (slow);
}

t_path		*merge_sort(t_path *head)
{
	t_path	*mid;
	t_path	*shalf;

	if (head == NULL || head->next == NULL)
		return (head);
	mid = get_mid(head);
	shalf = mid->next;
	mid->next = NULL;
	return (merge(merge_sort(head), merge_sort(shalf)));
}

void		*free_dir(char *dirname, t_list_path *list_names)
{
	ft_strdel(&dirname);
	free(list_names);
	return (NULL);
}

t_list_path	*g_readdir(DIR *dirp,
									int flags, char *pattern, char *dirname)
{
	t_list_path			*ln;
	struct dirent		*dp;

	ln = alloc_path_list(1);
	if (dirp == NULL)
		return ((t_list_path *)free_dir(dirname, ln));
	while ((dp = readdir(dirp)))
	{
		if (!ft_strequ(dp->d_name, ".") && !ft_strequ(dp->d_name, ".."))
		{
			if (!(dp->d_name[0] == '.' && pattern[0] != '.'))
			{
				if (flags & ISDIR)
				{
					if (dp->d_type == DT_DIR)
						psh(ln, ft_strdup(dp->d_name), ft_strlen(dp->d_name));
				}
				else
					psh(ln, ft_strdup(dp->d_name), ft_strlen(dp->d_name));
			}
		}
	}
	closedir(dirp);
	ft_strdel(&dirname);
	return (ln);
}
