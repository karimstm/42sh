/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjamal <cjamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 01:26:35 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/03/02 19:44:54 by cjamal           ###   ########.fr       */
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
# include <sys/stat.h>
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
# define PRINT_ERROR(s) ft_printf_fd(2, "42sh: %s: %s\n",s)
# define PRINT_ERROR2(s1,s2) ft_printf_fd(2, "42sh: %s: %s\n", s1, s2)

typedef struct			s_builtin
{
	char				*cmd;
	int					(*f)();
}						t_builtin;

typedef	struct			s_blt_line
{
	char				**env;
	t_list				*blt;
}						t_blt_line;

/*
** Structure for environement and set variables
*/
typedef struct			s_variables
{
	char				*key;
	char				*value;
	int					is_exported;
	int					is_modified;
	struct s_variables	*next;
}						t_variables;

typedef struct			s_variables_list
{
	t_variables			*head;
	t_variables			*tail;
	int					node_count;
}						t_variables_list;

t_variables_list		*g_env;

/*
** Structure for the test built-in
*/

typedef	enum			e_token_op
{
	TOKEN_OP_NONE,
	TOKEN_OP_Z,
	TOKEN_OP_STRING_EQ,
	TOKEN_OP_STRING_NE,
	TOKEN_OP_EQ,
	TOKEN_OP_NE,
	TOKEN_OP_GE,
	TOKEN_OP_GT,
	TOKEN_OP_LE,
	TOKEN_OP_LT,
}						t_token_op;

typedef struct			s_test
{
	t_token_op			op;
	char				*s1;
	char				*s2;
}						t_test;

typedef	struct			s_fifo
{
	char				*pathname;
	int					fd;
	pid_t				child;
	struct s_fifo		*next;
}						t_fifo;

typedef	struct			s_fifo_list
{
	t_fifo				*head;
	t_fifo				*tail;
	int					node_count;
}						t_fifo_list;

/*
**	============================= Expansion struct =============================
*/

typedef struct			s_parser_expansion
{
	char		*str;
	char		*index;
}						t_parser_expansion;

/*
**	=============================== MINISHELL ==================================
*/

/*
**	main.c
*/
void					restore_std(int std[3]);
t_node					*start_parsing_command(const char *line);
void					free_stacked_node(t_stack *sp, t_job_list *jobs);
t_alias_list			*get_alias_list(t_alias_list *list);
t_job_list				*get_job_list(t_job_list *jobs);
t_stack					*get_stack(t_stack *sp);

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
int						ft_set_var(char **args, t_simple_command *cmd);
int						ft_set(char **args);
int						ft_unset(char **args);
int						ft_export(char **args);
int						ft_pwd(char **args);

/*
** Builtin cd helpres
*/

int						is_correct_path(char *path, int print_error);
char					*get_path_var(const char *key, int get_current_dir);
char					*concat_path_with_cdpath(char *path);

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
void					save_history(void);
int						ft_history(char **args);
t_cmd_history			*get_specific_history(int index, char is_l);
t_cmd_history			*get_history_by_reverse(int max);
t_cmd_history			*get_specific_history_by_str(char *first,
							char *error_msg);
char					*search_history(char *format);
int						ft_fc(char **args);
int						fc_l(int flags, char *first, char *last);
int						fc_s(char *first, char *last);
int						fc_edit(int flags, char *editor, char *first,
							char *last);

/*
**	exec.c
*/

char					**node_to_char(t_list_simple_command *command);
char					*working_path(char *cmd);
t_job_list				*start_exec(t_node *node, t_list *env);
int						execute_redirection(t_redirection *list);
int						assert_tok(t_token_kind base, t_token_kind k2,
						t_token_kind k3);

/*
**	redir.c
*/

int						output(t_redirection *redir);
int						output_append(t_redirection *redir);
int						output_with_aggregate(t_redirection *redir,
							int is_append);
int						input(t_redirection *redir);
int						input_here_doc(t_redirection *redir);
int						input_with_aggregate(t_redirection *redir);
int						input_output(t_redirection *redir);
int						check_file_status(char *filename);
int						fd_is_valid(int fd);
int						bad_fd(int fd);
int						check_file_permission(char *filename, int perm);
int						check_file_status(char *filename);
int						dup4(int fd1, int fd2);

/*
**	JobTesting execute.c
*/

void					execute(t_job_list *job_list,
							t_node *node, t_list *blt);

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
void					execute_entry(t_job_list *job_list,
						t_node *node, t_blt_line *blt_line, t_job_kind kind);
int						run_built_in(t_blt_line *blt_line,
							t_process *process);
t_simple_command		*get_assignement_name(t_list_simple_command *list);
char					**get_assignements(t_list_simple_command *list);
void					handle_assigns(t_list_simple_command *node_cmd);

/*
**	quote_stripping.c
*/
char					*quote_stripping(char *str);

/*
**	fds.c
*/
int						dup3(int oldfd);
void					set_fds(int tmp_stds[3]);

/*
**	fds.c
*/
int						ft_type(char **args);
char					*is_aliased(char *arg);

/*
** env_list.c
*/
void					ft_init_env(char **ev);
t_variables				*get_var(char *target);
void					variable_push(char *key, char *value,
							int export, int is_modified);
void					delete_var(char *target);
char					**get_tab_env();
t_variables_list		*dup_env(void);
void					reset_env(t_variables_list *tmp,
							char **assign);
void					ft_del_env();
int						edit_add_var(char *key, char *value,
							int is_exported, int mod);
int						ft_var_isvalid(char *str);
int						ft_env(char **args);
/*
**	pre_parse.c
*/

char					*pre_parse(const char *line);

/*
** system.c
*/

void					init_shell_variables();
int						sh_system(char *name, char save_history);

/*
** ft_strsignal.c
*/
const char				*ft_strsignal(int sig);

/*
** ft_getopt.c
*/
int						ft_getopt(char **args, char *optstring, int *argc);

/*
** Execution
*/
void					execute_simple_command(t_job_list *job_list,
								t_blt_line *blt_line);
t_simple_command		*get_command_name(t_list_simple_command *list);
void					seperator_handling(t_job_list *job_list,
								t_node *node, t_blt_line *blt_line);
void					and_or_handling(t_job_list *job_list, t_node *node,
							t_blt_line *blt_line, t_job_kind kind);
void					execute_process(t_job *job, t_process *process,
								t_blt_line *blt_line, int pip[2]);

void					initial_process(pid_t pgid, t_job_kind kind);
int						setup_redirection(t_process *p, int doexit);
int						run_built_in(t_blt_line *blt_line, t_process *process);
void					setup_pgid(pid_t child, t_job *job);

/*
** Command_utils.c
*/
t_simple_command		*get_command_name(t_list_simple_command *list);
char					*command_path(char *name);
t_cmd_type				cmd_type(t_process *p);

/*
**	pre_parse.c
*/
char					*consume_word(const char **line, char c);

/*
** events.c
*/
void					command_line_n(const char **str, char c);
int						get_event(const char **new, t_string *str, char c);
int						spacial_cases(const char *line);
int						quotes_s(const char **real_line, t_string *str);
void					consume_double_quote(const char **line, t_string *str);
void					consume_single_quote(const char **line,
							t_string *tstring);

/*
** Test command
*/

int						do_file_checking(char *filename, int flag);
int						expression(char **argv);
int						ft_test(char **argv);
t_token_op				get_token_op(char *tmp);
int						ft_op_error(char *s);
int						ft_integer_error(char *s);

/*
** Expansion Core
*/

int						expand_args(t_list_simple_command *args);
char					*expand(char *str, t_parser_expansion (*f)(char *));

/*
** Sub process
*/

void					scan_process();
void					init_scan_process();
char					*process_sub_parser(char *name);
void					setup_sub_proce(t_list_simple_command *list);
void					init_fifo_list(t_fifo_list *list);
char					*ft_tpname(const char *tmpdir);
void					fifo_push(t_fifo_list *list, char *pathname,
							int fd, pid_t child);
void					close_fifos(t_fifo_list *list);
char					*dev_path(int fd);
t_fifo_list				*fifo_list(t_fifo_list *list);
int						skip_dqoute(char **string, char c);
pid_t					setup_proc(char *cmd, char *path, int flag);
char					*scan_pro(char *name);
char					*setup_fifo();

/*
** General expansion
*/
void					init_expansion(t_list_simple_command *list);
int						setup_expan(t_list_simple_command *list);
void					expand_redirection(t_redirection *redir);
void					clean_list(t_list_simple_command	*list);

/*
** back_slash.c
*/
int						back_slash_escape(char **string,
						t_string *str, int include);

#endif
