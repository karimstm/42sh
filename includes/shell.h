			   /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 01:26:35 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/05 10:47:34 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "const.h"
# include <term.h>
# include "../readline/ft_readline.h"
# include "libft.h"
# include "quote.h"
# include "ast.h"
# include "stack.h"
# include "parse.h"
# include "lex.h"
# include <dirent.h>
# include <termios.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <dirent.h>
# include <glob.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/random.h>
# include "jobs.h"
# include "alias.h"
# include "hash.h"

# undef tab
# define BUF_S 1000
# define L_FLAG 0x00000001
# define N_FLAG 0x00000002
# define R_FLAG 0x00000004
# define S_FLAG 0x00000008
# define BREAK_FLAG 0x00000010

typedef struct			s_builtin
{
	char				*cmd;
	int					(*f)();
}						t_builtin;

typedef	struct			s_blt_line
{
	char                **env;
	t_list				*blt;
}						t_blt_line;

/* Structure for environement and set variables */

typedef struct			s_variables
{
	char                *key;
	char                *value;
	int               	is_exported;
	int                 is_modified;
	struct s_variables  *next;
}						t_variables;

typedef struct			s_variables_list
{
	t_variables         *head;
	t_variables         *tail;
	int                 node_count;
}						t_variables_list;

t_variables_list	*env2;

/*
**	=============================== MINISHELL ==================================
*/

/*
**	main.c
*/
void					restore_std(int std[3]);

/*
**	shell.c
*/
int						is_directory(const char *path);


/*
**	builtin.c
*/
void					init_builtin(t_list **lst);
int						check_builtin(t_list *elem, void *obj);
void					free_builtin(t_list *lst);
t_list					*get_set_blt(t_list	*blt);


/*
**	free.c
*/
void					free_exec_cmd(char *error, char *full_path,
		char **head_path);
void					free_elem_env(t_list *elem);


/*
**	builtins commands.
*/
int						ft_cd(char **args);
int						ft_echo(char **args);
int						ft_env(char **args);
int                     ft_set_var(char **args);
int						ft_set(char **args);
int						ft_unset(char **args);
int						ft_export(char **args);
int						ft_pwd(char **args);


/*
**	handlers.c
*/
struct termios			*get_termios(void);
void					sig_handler(int sig);
void					child_handler(int sig);
void					signals(void);
void					exit_shell(char *format, ...);

/*
**	history.c
*/
void					restore_history(void);
void                    save_history(void);
int                     ft_history(char **args);
int                     ft_fc(char **args);
void                    fc_l(int flags, char *first, char *last);
void                    fc_s(int flags, char *first, char *last);
void                    fc_edit(int flags, char *editor, char *first, char *last);

/*
**	exec.c
*/

char					**node_to_char(t_list_simple_command *command);
char					*working_path(char *cmd);
t_job_list				*start_exec(t_node *node, t_list *env);
int						execute_redirection(t_redirection *list);

/*
**	redir.c
*/

int						output(t_redirection *redir);
int						output_append(t_redirection *redir);
int						output_with_aggregate(t_redirection *redir, int is_append);
int						output_with_aggregate_append(t_redirection *redir);
int						input(t_redirection *redir);
int						input_here_doc(t_redirection *redir);
int						input_with_aggregate(t_redirection *redir);
int						input_output(t_redirection *redir);
int						check_file_status(char *filename);

/*
**	JobTesting execute.c
*/

void					execute(t_job_list *job_list, t_node *node, t_list *blt);

/*
** ft_jobs.c
*/
int						ft_jobs(char **args);

/*
** ft_exit.c
*/
int						ft_exit(char **cmds);

/*
**	execute.c
*/

t_job_list				*get_job_list(t_job_list *jobs);
void					execute_entry(t_job_list *job_list, t_node *node, t_blt_line *blt_line, t_job_kind kind);
int						run_built_in(t_blt_line *blt_line, t_process *process);
t_simple_command        *get_assignement_name(t_list_simple_command *list);

/*
**	quote_stripping.c
*/

char					*quote_stripping(char *str);

/*
**	fds.c
*/

int						dup3(int oldfd);

/*
**	fds.c
*/
int						ft_type(char **args);
char    				*is_aliased(char *arg);

/*
** env_list.c
*/
void					ft_init_env(char **ev);
t_variables 			*get_var(char *target);
void					variable_push(char *key, char *value, int export, int is_modified);
void					delete_var(char *target);
char					**get_tab_env();
t_variables_list        *dup_env(void);
void					reset_env(t_variables_list *tmp, char **assign);

/*
**	pre_parse.c
*/

char					*pre_parse(const char *line);

#endif
