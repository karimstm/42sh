/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reg_match.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:26:35 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/03/02 16:56:38 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_pattern	*split_pattern(char *patrn)
{
	t_pattern	*head;
	t_pattern	**ptr;
	char		*tmp;

	head = NULL;
	ptr = &head;
	while (*patrn && (*ptr = ft_memalloc(sizeof(t_pattern))))
	{
		if ((tmp = get_matched_bracket(patrn, "[]", "[", 1)) && patrn++)
			(*ptr)->type = PT_CHAR_SEQ;
		else if (*patrn == '^' && head->next == NULL && (tmp = patrn))
			(*ptr)->type = PT_START;
		else if (*patrn == '*' && (tmp = patrn))
			(*ptr)->type = PT_STAR;
		else if (*patrn == '?' && (tmp = patrn))
			(*ptr)->type = PT_QMARK;
		else if (*patrn == '$' && patrn[1] == 0 && (tmp = patrn))
			(*ptr)->type = PT_END;
		else if ((tmp = patrn))
			(*ptr)->type = PT_CHAR;
		(*ptr)->str = ft_strsub(patrn, 0, tmp - patrn + ((*ptr)->type != 4));
		patrn = tmp + 1;
		ptr = &(*ptr)->next;
	}
	return (head);
}

static char			*pattern_sanitize(char *pattern)
{
	char	*sanitized_string;
	int		i;

	sanitized_string = pattern;
	i = 0;
	while (*pattern)
	{
		if (*pattern == '\\')
			sanitized_string[i++] = *++pattern;
		else if (!(*pattern == '*' && pattern[1] == '*'))
			sanitized_string[i++] = *pattern;
		if (*pattern)
			pattern++;
	}
	sanitized_string[i++] = 0;
	return (sanitized_string);
}

t_list				*get_star_match(char *str, t_pattern *pattern)
{
	t_list	*ret;

	ret = NULL;
	if (pattern->next && pattern->next->type == PT_END)
	{
		while (*str)
			str++;
		if ((ret = ft_memalloc(sizeof(t_list))))
			ret->content = str;
	}
	else
	{
		while (1)
		{
			ft_lstenqueue(&ret, is_matched(str, pattern->next));
			if (!*str)
				break ;
			str++;
		}
	}
	return (ret);
}

t_list				*is_matched(char *str, t_pattern *pattern)
{
	t_list	*ret;
	int		matched;

	matched = 1;
	pattern = pattern && pattern->type == PT_START ? pattern->next : pattern;
	while (pattern && *str && matched)
	{
		if (pattern->type == PT_STAR)
			return (get_star_match(str, pattern));
		else if (pattern->type == PT_QMARK)
			str++;
		else if (pattern->type == PT_CHAR)
			*(pattern->str) == *str ? (int)*str++ : matched--;
		else if (pattern->type == PT_CHAR_SEQ)
			ft_isinstr(*str, pattern->str) ? (int)*str++ : matched--;
		else
			return (NULL);
		matched ? (pattern = pattern->next) : 0;
	}
	if ((pattern && pattern->type != 5) || !(ret = ft_memalloc(sizeof(t_list))))
		return (NULL);
	ret->content = str;
	return (ret);
}

t_list				*reg_match(char *str, char *str_pattern)
{
	t_matched_strings	ptr;
	t_list				*starts;
	t_list				*ends;
	t_pattern			*pattern;

	starts = NULL;
	if ((pattern = split_pattern(pattern_sanitize(str_pattern))))
		while (*str)
		{
			if ((ends = is_matched(str, pattern)))
			{
				ptr.start = str;
				ptr.ends = ends;
				ft_lstenqueue(&starts, ft_lstnew(&ptr,
													sizeof(t_matched_strings)));
			}
			str++;
			if (pattern->type == PT_START)
				break ;
		}
	free_pattern(pattern);
	return (starts);
}
