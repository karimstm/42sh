/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 15:52:19 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/11 14:47:35 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include <ctype.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <assert.h>
# include <stdarg.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

typedef struct s_node		t_node;
typedef struct s_sep_op		t_sep_op;
typedef struct s_and_or		t_and_or;

typedef enum				e_token_kind {
	TOKEN_EOF = 0,
	TOKEN_DQUOTE,
	TOKEN_SQUOTE,
	TOKEN_LBRACE,
	TOKEN_RBRACE,
	TOKEN_BANG,
	TOKEN_SPACE,
	TOKEN_LAST_CHAR = 127,
	TOKEN_DIGIT,
	TOKEN_WORD,
	TOKEN_WORD_DQUOTED,
	TOKEN_WORD_SQOUTED,
	TOKEN_ASSIGNMENT_WORD,
	TOKEN_NAME,
	TOKEN_NEWLINE,
	TOKEN_AND_IF,
	TOKEN_OR_IF,
	TOKEN_DSEMI,
	TOKEN_DLESS,
	TOKEN_DGREAT,
	TOKEN_LESSAND,
	TOKEN_ANDGREAT,
	TOKEN_AND_DEGREATE,
	TOKEN_GREATAND,
	TOKEN_DGREATAND,
	TOKEN_LESSGREAT,
	TOKEN_DLESSDASH,
	TOKEN_HERESTRING,
	TOKEN_CLOBBER,
	TOKEN_IF,
	TOKEN_THEN,
	TOKEN_ELSE,
	TOKEN_ELIF,
	TOKEN_FI,
	TOKEN_DO,
	TOKEN_DONE,
	TOKEN_CASE,
	TOKEN_ESAC,
	TOKEN_WHILE,
	TOKEN_UNTIL,
	TOKEN_FOR,
	TOKEN_IN,
}							t_token_kind;

typedef enum				e_compound_kind
{
	NONE,
	NODE,
	SIMPLE_COMMAND
}							t_compound_kind;

typedef enum				e_cmd_kind {
	CMD_NONE,
	CMD_SIMPLE,
	CMD_BREAK,
	CMD_CONTINUE,
	CMD_BLOCK,
	CMD_IF,
	CMD_WHILE,
	CMD_DO_WHILE,
	CMD_FOR,
	CMD_CASE,
	CMD_ASSIGN,
	CMD_UNTIL,
}							t_cmd_kind;

typedef enum				e_cmd_type {
	IS_NONE,
	IS_BUILTIN,
	IS_PATH_CMD,
	IS_FOUND,
	IS_NOTFOUND,
}							t_cmd_type;

typedef struct				s_simple_command {
	t_token_kind			kind;
	char					*name;
	t_cmd_type				type;
	struct s_simple_command	*next;
}							t_simple_command;

/*
** Saves the head and the tail of a simple command args
** DO NOT MISTAKE THIS WITH COMPOUND COMMAND
*/
typedef struct				s_list_simple_command
{
	t_simple_command		*head;
	t_simple_command		*tail;
	size_t					node_count;
}							t_list_simple_command;

/*
** t_token_kind kind; redirection kind
*/
typedef struct				s_redirection
{
	t_token_kind			kind;
	int						fd1;
	int						fd2;
	char					*word;
	char					*herdoc;
	struct s_redirection	*next;
	struct s_redirection	*prev;

}							t_redirection;

/*
** kind could ben either '&' or ';
*/
struct						s_sep_op {
	t_token_kind			kind;
	t_node					*left;
	t_node					*right;
};

/*
** kind could be either '||', '&&' or '|'
*/
struct						s_and_or {
	t_token_kind			kind;
	t_node					*left;
	t_node					*right;
};

struct						s_assignment {
	char					*name;
	t_node					*left;
};

typedef enum				e_token_node {
	NODE_AND_OR,
	NODE_SEMI_AND,
	NODE_PIPE,
	NODE_SIMPLE_COMMAND,
}							t_token_node;

typedef enum				e_grouping_kind {
	GROUP_NONE = 0,
	GROUP_BRACE_COMMAND,
	GROUP_PARAN_COMMAND,
}							t_grouping_kind;

typedef union				u_node_spec
{
	t_sep_op				*sep_op_command;
	t_and_or				*and_or_command;
	t_list_simple_command	*simple_command;
}							t_node_spec;

struct						s_node
{
	t_token_node	kind;
	t_grouping_kind	goup_kind;
	t_redirection	*redir;
	t_node_spec		spec;
};

/*
** 	AST.C
*/
t_list_simple_command		*malloc_list_simple_command();
void						init_list_simple_command(t_list_simple_command *s);
void						token_push(t_list_simple_command *list,
											char *token_str, t_token_kind kind);
t_list_simple_command		*merge_list(t_list_simple_command *left,
									t_list_simple_command *right);
void						free_list(t_list_simple_command *list);
void						free_tree(t_node **node);
void						free_redir(t_redirection **redir);
t_node						*command_node(t_token_node kind);
void						print_list_tokens(t_list_simple_command *list);
t_sep_op					*sep_commands(t_token_kind kind, t_node *left,
											t_node *right);
t_and_or					*and_or_commands(t_token_kind kind,
									t_node *left, t_node *right);
void						*xmalloc(size_t num_bytes);
t_redirection				*new_redir(int fd1, int fd2, t_token_kind kind);
t_redirection				*reverse_redirection(t_redirection *list);
void						print_redir(t_redirection *list);

char						*ft_tmpfile();
int							open_tmp(char *pathname);
#endif
