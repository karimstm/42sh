/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 16:13:30 by amoutik           #+#    #+#             */
/*   Updated: 2020/01/25 12:51:14 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEX_H
# define LEX_H
# include "ast.h"

typedef union		u_spec
{
	int			int_val;
	char		*word;
	char		*name;
}					t_spec;

typedef struct		s_tokens
{
	t_token_kind	kind;
	const char		*line;
	const char		*start;
	const char		*end;
	t_spec			spec;
}					t_tokens;

t_tokens			g_token;

const char			*g_line;

/*
** main.c
*/

void				init_stream(const char *str);
int					match_token(t_token_kind kind);
void				next_token();
int					is_token(t_token_kind kind);
void				escape_space();
void				syntax_error(const char *fmt, ...);
int					*error_num(void);
void				reset_error_num(void);
int					get_new_line(void);

#endif
