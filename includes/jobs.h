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
	J_BACKGROUND = 1
}				t_job_kind;

/* A job is a pipeline of processes.  */
typedef struct s_job
{
	struct		s_job *next;           /* next active job */
	char		*command;              /* command line, used for messages */
	t_list_process	*proc_list;     /* list of processes in this job */
	t_list			*env;				/* environment */
	pid_t		pgid;                 /* process group ID */
	pid_t		sid;				/* Process session ID */
	char 		notified;              /* true if user told about stopped job */
	struct termios	tmodes;      /* saved terminal modes */
	t_job_kind		kind;		/* JOB_KIND J_NOTIFIED, J_FOREGROUND, J_BACKGROUND */
	int stdin, stdout, stderr;  /* standard i/o channels */
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
t_job_list		*get_job_list(t_job_list *jobs);
void			dumpy_process(t_job_list *job_list, t_node *node);
void			job_notification(t_job_list *jobs);

void			init_shell();
void			wait_for_job (t_job *j);
void			put_job_in_foreground (t_job *j, int cont);
void			put_job_in_background (t_job *j, int cont);
void			do_job_notification (t_job_list *jobs);
void			format_job_info (t_job *j, const char *status);
void			launch_process(t_process *p, pid_t pgid, int infile, int outfile, int errfile, int foreground, char **env);

#endif



struct job {
    pid_t gleader;		/* process group leader of this job  */
    int stat;                   /* see STATs below                   */
    char *pwd;			/* current working dir of shell when * this job was spawned              */
    struct s_process *procs;	/* list of processes                 */
    int stty_in_env;		/* if STTY=... is present            */
};
