/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_remove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 04:41:04 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/21 01:10:23 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static long matched_len(char *str, char *patrn, long val, long(*f)(long, long))
{
    t_list *ret;
    t_list *ends;
    t_list *tmp;
    t_matched_strings *ptr;

    if(!(ret = reg_match(str, patrn)))
        return(0);
    while (ret)
    {
        ptr = (t_matched_strings *)ret->content;
        ends = ((t_matched_strings *)ret->content)->ends;
        while (ends)
        {
            val = f((char *)ends->content - ptr->start, val);
            tmp = ends;
            ends = ends->next;
            free(tmp);
        }
        tmp = ret;
        ret = ret->next;
        free(tmp->content);
        free(tmp);
    }
    return(val);
}

char *expand_rem_pre(char *var_name, char *expr, long val, long(*f)(long, long))
{
    char *ret;
    char *tmp;
    long cut_len;
    t_variables *var;


    ret = NULL;
    if(!(var = get_var(var_name)))
        return(ft_strdup(""));
    if ((tmp = expand(ft_strjoin("^", ft_skip_chars(expr, "#", NULL)), NULL)))
    {
        cut_len = matched_len(var->value, tmp, val, f);
        free(tmp);
        return (ft_strsub(var->value,cut_len, ft_strlen(var->value)));
    }
    return(ret);
}

char *expand_rem_suf(char *var_name, char *expr, long val, long(*f)(long, long))
{
    char *ret;
    char *tmp;
    long cut_len;
    t_variables *var;


    ret = NULL;
    if(!(var = get_var(var_name)))
        return(ft_strdup(""));
    if ((tmp = expand(ft_strjoin(ft_skip_chars(expr, "%", NULL), "$"), NULL)))
    {
        cut_len = matched_len(var->value, tmp, val, f);
        free(tmp);
        return (ft_strsub(var->value, 0, ft_strlen(var->value) - cut_len));
    }
    return(ret);
}