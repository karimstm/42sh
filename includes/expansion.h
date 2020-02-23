/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 00:09:49 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/23 02:42:47 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
#define EXPANSION_H

#include "shell.h"
#include <limits.h>
#include <pwd.h>

enum{
	MIN,MAX
};

char					*expand_string_lenght(char *var_name, char *expression);
char					*expand_simple(char *var_name);

char					*expand_defaul_val(char *var_name, char *expression);
char					*expand_alternative_val(char *var_name, char *expression);
char					*assign_default_val(char *var_name, char *expression);
char					*indicate_error(char *var_name, char *expression);

char					*expand_rem_pre(char *key, char *expr, long val, long(*f)(long, long));
char					*expand_rem_suf(char *key, char *expr, long val, long(*f)(long, long));

char					*cmd_substitution(const char *cmd);
char					*expand_ar_expr(char *expr);

t_parser_expansion		expand_sub_art(char *str);
t_parser_expansion		expand_parametre(char *str);

#endif