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
# include "libft.h"
# include "quote.h"
# include "ast.h"
# include "stack.h"
# include "parse.h"
# include "lex.h"
# include <dirent.h>
# include <term.h>
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

# define BUF_S 1000

typedef struct			s_builtin
{
	char				*cmd;
	int					(*f)();
}						t_builtin;

typedef struct			s_line
{
	char				*command;
	char				*copy;
	int					buf_size;
	int					col;
	int					top;
	int					index;
	int					current_index;
	t_list				*new_lines;
	t_list				*head_newlines;
	char				print_msg;
	t_list				**tail_history;
	t_list				*index_history;
	char				*tmp_history;
	t_list				*env;
}						t_line;

typedef	struct			s_blt_line
{
	t_list				*blt;
	t_line				*line;
}						t_blt_line;

/* Structure for environement and set variables */

typedef struct			s_variables
{
    char                *key;
    char                *value;
    int               	is_exported;
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
int						ft_set(char **args);
int						ft_unset(char **args);
int						ft_export(char **args);
int						ft_pwd(char **args);

/*
**	=============================== READLINE ==================================
*/

/*
**	copy.c
*/
void					handle_copy(t_line *line, int key);

/*
**	cursor.c
*/
void					go_left(t_line *line);
void					go_right(t_line *line);
void					move_cursor(int direction, t_line *line);

/*
**	cursor2.c
*/
int						get_current_row(int height);
void					go_home(t_line *line);
void					go_home_line(t_line *line);
void					go_end(t_line *line);
void					go_end_line(t_line *line);

/*
**	cursor3.c
*/
void					go_down(t_line *line);
void					go_up(t_line *line);
void					next_word(t_line *line, int direction);
void					update_line(t_line *line, char *tmp, char buf);

/*
**	cursor4.c
*/
void					update_index(t_line *line, char step);
int						decision_up_down(t_line *line);
int						get_current_rows(t_line *line);
int						decision_top_down_left(t_line *line, int current_rows);
void					set_new_current_index(t_line *line);

/*
**	edit_line.c
*/
void					print_newchar(t_line *line, int buf);
t_list					*free_next_newlines(t_line *line);
void					print_char_inline(t_line *line, int buf);
void					go_down_left(void);

/*
**	handlers.c
*/
void					sig_handler(int sig);
void					child_handler(int sig);
void					signals(void);
void					exit_shell(char *format, ...);

/*
**	history.c
*/
void					handle_history(int buf, t_line *line);
void					m_add_history(t_line *line);

/*
**	line.c
*/
t_line					*get_t_line(void);
void					free_line(void);
t_line					*init_line(void);
void					handle_eot(t_line *line);

/*
**	paste.c
*/
void					print_pasted_chars(int *buf, t_line *line);
void					internal_paste(t_line *line);

/*
**	read_line.c
*/
void					clr_screen(int sig);
int						read_line(t_line *line);

/*
**	terms.c
*/
struct termios			*get_termios(void);
int						init_termios(struct termios term);
int						init_terms(void);

/*
**	exec.c
*/

int						execute_cmd(t_node *node, t_list *blt, t_line *line, t_job_kind kind, t_list_process *p);
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

void					execute(t_job_list *job_list, t_node *node, t_line *line, t_list *blt);

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
void					variable_push(char *key, char *value, int export);
void					delete_var(char *target);
char					**get_tab_env();


#endif
