/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reg_match.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:26:35 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/20 02:04:18 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// dir bhsab l escaping a molay
t_list *is_matched(char *str, t_pattern *pattern);

static t_pattern *split_pattern(char *pattern)
{
	t_pattern *head;
	t_pattern **ptr;
	char *tmp;

	head = NULL;
	ptr = &head;
	ft_printf("pattern : `%s`\n", pattern);
	while (*pattern && (*ptr = ft_memalloc(sizeof(t_pattern))))
	{
		if ((tmp = get_matched_bracket(pattern, "[]", "[", 1)) && pattern++)
			(*ptr)->type = PT_CHAR_SEQ;
		else if (*pattern == '^' && head->next == NULL && (tmp = pattern))
			(*ptr)->type = PT_START;
		else if (*pattern == '*' && (tmp = pattern))
			(*ptr)->type = PT_STAR;
		else if (*pattern == '?' && (tmp = pattern))
			(*ptr)->type = PT_QMARK;
		else if (*pattern == '$' && pattern[1] == 0 && (tmp = pattern))
			(*ptr)->type = PT_END;
		else if ((tmp = pattern))
			(*ptr)->type = PT_CHAR;
		(*ptr)->str = ft_strsub(pattern, 0, tmp - pattern + ((*ptr)->type != 4));
		ft_printf("%s, %d\n", (*ptr)->str, (*ptr)->type);
		pattern = tmp + 1;
		ptr = &(*ptr)->next;
	}
	return (head);
}

static char *pattern_sanitize(char *pattern)
{
	char *sanitized_string;
	int i;

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

t_list *get_star_match(char *str, t_pattern *pattern)
{
	t_list *ret;

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
			if(!*str)
				break;
			str++;
		}
	}
	return (ret);
}

t_list *is_matched(char *str, t_pattern *pattern)
{
	t_list *ret;
	int matched;

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
	if ((pattern && pattern->type != PT_END) || !(ret = ft_memalloc(sizeof(t_list))))
		return (NULL);
	ret->content = str;
	return (ret);
}

t_list *reg_match(char *str, char *str_pattern)
{
	t_matched_strings ptr;
	t_list *starts;
	t_list *ends;
	t_pattern *pattern[2];

	starts = NULL;
	if ((pattern[0] = split_pattern(pattern_sanitize(str_pattern))))
		while (*str)
		{
			if ((ends = is_matched(str, pattern[0])))
			{
				ptr.start = str;
				ptr.ends = ends;
				ft_lstenqueue(&starts, ft_lstnew(&ptr, sizeof(t_matched_strings)));
			}
			str++;
			if (pattern[0]->type == PT_START)
				break;
		}
	while (pattern[0])
	{
		pattern[1] = pattern[0];
		pattern[0] = pattern[0]->next;
		free(pattern[1]->str);
		free(pattern[1]);
	}
	return (starts);
}