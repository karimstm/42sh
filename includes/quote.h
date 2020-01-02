/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 09:42:19 by amoutik           #+#    #+#             */
/*   Updated: 2019/05/11 18:53:47 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_H
# define QUOTE_H
# include "libft.h"
# define EOS '\0'
# define SH_TOKEN "<>&|;"

/*
**! \enum token_type
**
**  It better to use enum to check word type
*/

enum					e_token_type
{
	SH_WORD = 1,
	SH_PIPE = 2,
	SH_REDIRECTION = 4,
	SH_SEMI = 8,
	SH_QUOTED = 16,
	SH_DPIPE = 32,
	SH_GLOBE = 64,
	SH_EXPORT = 128
};

typedef struct			s_token
{
	char				*token;
	enum e_token_type	tok_type;
	struct s_token		*next;
}						t_token;

typedef struct			s_token_list
{
	struct s_token		*head;
	struct s_token		*tail;
	int					node_count;
}						t_token_list;

typedef struct			s_string
{
	char				*string;
	size_t				cap;
	size_t				len;
}						t_string;

/*
** Quote.c
*/

int						split_tok(t_token_list *list,
		char **ptr, t_string *str, enum e_token_type type);
t_token_list			*handle_quote(char **line);

/*
** quote2.c
*/

int						check_quote_ending(char **line);
int						split_quote(t_token_list *list, char **ptr,
		t_string *str, enum e_token_type type);

/*
** string.c
*/

void					new_string(t_string *str);
void					push(t_string *str, char c);
char					space(char c);
int						insert_token(t_token_list *list,
		t_string *str, enum e_token_type type);

/*
** til_dol.c
*/

int						handle_dollar(char **ptr, t_string *str);
int						handle_tilda(char **ptr, t_string *str);

/*
** List.c
*/
void					init_token_list (t_token_list *list);
void					push_token(t_token_list *list,
		char *tok, enum e_token_type type);
void					delete_token(t_token_list *list, t_token *tofind);
char					free_token_list(t_token_list *list);
t_token					*insert_token_indexed(t_token *current, char *str, enum e_token_type type);

/*
** free_string.c
*/

void					free_token(t_token *node);
int						ret_with_str_free(t_string *str, int ret);
void					free_tokens(t_token_list *list);
void					free_string(t_string *str);

/*
** error.c
*/

int						parse_error(char *ptr, int err);
int						check_syntax_error(t_token_list *tokens);

/*
** is_number.c
*/

int						is_number(char *str);

/*
** util.c
*/

char					check_pipe(t_token *token);
void					next_pipe(t_token_list *tokens);
t_token					*get_cmd_token(t_token *ptr);
char					**list_to_chars(t_token *token);

/*
** spliters.c
*/

int						is_special_char(char c);
int						is_pipe_at_end(t_token_list *tokens);


//To be deleted
void					print_tokens(t_token_list *list);
#endif
