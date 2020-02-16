/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 16:13:30 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 17:40:38 by amoutik          ###   ########.fr       */
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

/*
** variabel current is used here to peserve the real
** value that will be put in the history
*/

typedef struct		s_tokens
{
	t_token_kind	kind;
	const char		*line;
	char			*current;
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
int					get_new_line(void);

void				scan_squotes(void);
void				scan_dquotes(void);
void				scan_meta_condition(char k1, t_token_kind k2);
void				scan_redirection_input(void);
void				scan_redirection_output(void);
void				scan_meta_semi(void);
void				scan_meta(void);
void				scan_curly(void);
void				check_assignment(void);
void				scan_dollar(void);
void				scan_string(void);
void				scan_int(void);
int					is_metacharacter(int ch);
int					is_ifs(char c);
uint8_t				get_digit(uint8_t index);

#endif
