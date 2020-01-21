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

pid_t		shell_pgid;
struct		termios shell_tmodes;
int			shell_terminal;
int			shell_is_interactive;


/* A process is a single process.  */
typedef struct 	s_process
{
	struct s_process *next;       /* next process in pipeline */
	char **argv;                /* for exec */
	char *command;				/* command name it could be somethign *
								* like this (ls -la && echo karim)*/
	t_node *node;				/* for exec */
	pid_t pid;                  /* process ID */
	char completed;             /* true if process has completed */
	char stopped;               /* true if process has stopped */
	int status;                 /* reported status value */
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

/* A job is a pipeline of processes.  */
typedef struct s_job
{
	struct				s_job *next;           /* next active job */
	char				*command;              /* command line, used for messages */
	t_list_process		*proc_list;     /* list of processes in this job */
	t_list				*env;				/* environment */
	pid_t				pgid;                 /* process group ID */
	pid_t				sid;				/* Process session ID */
	int					pos;
	t_job_current		current;
	char 				notified;              /* true if user told about stopped job */
	struct termios		tmodes;      /* saved terminal modes */
	t_job_kind			kind;		/* JOB_KIND J_NOTIFIED, J_FOREGROUND, J_BACKGROUND */
	int					stdin;
	int					stdout;
	int					stderr;  /* standard i/o channels */
}				t_job;

typedef struct	s_job_list
{
	t_job 	*head;
	t_job 	*tail;
	int		status;
	int		node_count;
}				t_job_list;

// Should not be here

t_job_list jobs;

/*
**
*/
void			process_push(t_list_process *list, pid_t pid, char **args, t_node *node);
void			init_process_list(t_list_process *p);
void			job_push(t_job_list *jobs, t_list_process *p, pid_t pgid);
void			init_job_list(t_job_list *jobs);
void			dumpy_process(t_job_list *job_list, t_node *node);
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
void			ft_fg(char **args, t_list **env);
void			ft_bg(char **args, t_list **env);
t_job			*get_job(t_job_list *jobs, int job_number);
int				get_min_pos(t_job_list *job_list);

#endif



struct job {
    pid_t gleader;		/* process group leader of this job  */
    int stat;                   /* see STATs below                   */
    char *pwd;			/* current working dir of shell when * this job was spawned              */
    struct s_process *procs;	/* list of processes                 */
    int stty_in_env;		/* if STTY=... is present            */
};
