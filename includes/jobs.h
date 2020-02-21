#ifndef JOBS_H
# define JOBS_H

# include <sys/types.h>
# include <termios.h>
# include <unistd.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>

# include <sys/ioctl.h>
# include <unistd.h>
# include "ast.h"
# include "libft.h"

pid_t		g_shell_pgid;
int			shell_terminal;


/*
** A process is a single process.
*/
typedef struct 	s_process
{
	struct s_process *next;
	char *command;				/* command name it could be somethign *
								* like this (ls -la && echo karim)*/
	t_node *node;				/* for exec */
	pid_t pid;					/* process ID */
	char completed;				/* true if process has completed */
	char stopped;				/* true if process has stopped */
	char signaled;				/* true if process has stopped due to a signal */
	int status;					/* reported status value */
}				t_process;


typedef struct	s_list_process
{
	t_process	*head;
	t_process	*tail;
	int			node_count;
}				t_list_process;


typedef enum	e_job_kind
{
	J_FOREGROUND = 0,
	J_BACKGROUND = 1,
	J_NON_INTERACTIVE = 2,
}				t_job_kind;

typedef enum	e_job_current
{
	CURRENT_NONE,			/* EMPTY */
	CURRENT_ACTIVE,			/* + */
	CURRENT_PREV			/* - */
}				t_job_current;

/*
** A job is a pipeline of processes.
** Think of a job as a group with a pipeline of processes
** Fortunatly I don't have to deal with sessions in here `sid`
** notified: true if user told about stopped job
** kind: JOB_KIND J_NOTIFIED, J_FOREGROUND, J_BACKGROUND
*/
typedef struct s_job
{
	struct				s_job *next;
	char				*command;
	t_list_process		*proc_list;
	t_list				*env;
	pid_t				pgid;
	pid_t				sid;
	int					pos;
	t_job_current		current;
	char 				notified;
	struct termios		tmodes;
	t_job_kind			kind;
}				t_job;

typedef struct	s_job_list
{
	t_job 	*head;
	t_job 	*tail;
	int		status;
	int		node_count;
}				t_job_list;



void			process_push(t_list_process *list, pid_t pid, t_node *node);
void			init_process_list(t_list_process *p);
void			job_push(t_job_list *jobs, t_list_process *p, pid_t pgid);
void			init_job_list(t_job_list *jobs);
void			job_notification(t_job_list *jobs);
int				mark_process_status(t_job_list *jobs ,pid_t pid, int status);
void			delete_last(t_job_list *list);
void			delete_job(t_job_list *list, t_job *target);

void			init_shell();
void			wait_for_job (t_job *j);
void			put_job_in_foreground (t_job *j, int cont);
void			put_job_in_background (t_job *j, int cont);
void			do_job_notification (t_job_list *jobs);
void			format_job_info (t_job *j);
void			launch_process(t_process *p, pid_t pgid, int infile, int outfile, int errfile, int foreground, char **env);

void			foreground_job(t_job_list *job_list, t_job *job, int cont);
void			background_job(t_job *job, int cont);
void			dummy_process(t_job_list *job_list, t_node *node, t_job_kind kind);
void			job_waiting(t_job_list *job_list, t_job *job);

void			ft_print_node(t_node *node);
const char		*ft_strsignal(int sig);

void			continue_job(t_job_list *job_list, t_job *j, int foreground);
int				ft_fg(char **args);
int				ft_bg(char **args);
t_job			*get_job(t_job_list *jobs, int job_number);
int				get_min_pos(t_job_list *job_list);
int				ft_tcsetpgrp(int fd, pid_t pgrp_id);
int				is_job_stopped(t_job *j);
int				is_job_completed(t_job *j);
void			set_active_job2(t_job_list *jobs, t_job *target);
void			free_job_list(t_job_list *jobs);
void 			update_status (t_job_list *jobs);
void			set_max_as_active(t_job_list *jobs);
void			delete_job(t_job_list *list, t_job *target);
t_job			*find_job(t_job_list *list, t_job *job, t_job **prev);
void			free_job_list(t_job_list *jobs);
void			free_proc(t_list_process **proces);
void			free_job(t_job *job);
pid_t			ft_tcgetpgrp(int fd);
int				ft_tcsetpgrp(int fd, pid_t pgrp_id);
char			current_to_char(t_job_current current);
void			print_pipes(t_process *process);
int				jobs_usage(void);
int				ft_jobs_not_found(char *args);
/*
** JOB buil-in command
*/
int				ft_job(char **args);


#endif
