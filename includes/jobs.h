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
# include "shell.h"

pid_t		shell_pgid;
struct		termios shell_tmodes;
int			shell_terminal;
int			shell_is_interactive;


/* A process is a single process.  */
typedef struct 	s_process
{
	struct s_process *next;       /* next process in pipeline */
	char **argv;                /* for exec */
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


/* A job is a pipeline of processes.  */
typedef struct s_job
{
	struct		s_job *next;           /* next active job */
	char		*command;              /* command line, used for messages */
	t_list_process	*proc_list;     /* list of processes in this job */
	pid_t		pgid;                 /* process group ID */
	pid_t		sid;				/* Process session ID */
	char 		notified;              /* true if user told about stopped job */
	struct termios	tmodes;      /* saved terminal modes */
	int stdin, stdout, stderr;  /* standard i/o channels */
}				t_job;

typedef struct	s_job_list
{
	t_job 	*head;
	t_job 	*tail;
	int		node_count;
}				t_job_list;

/*
**
*/
void		process_push(t_list_process *list, pid_t pid, char **args, t_node *node);
void		init_process_list(t_list_process *p);
void		job_push(t_job_list *jobs, pid_t pgid);
void		init_job_list(t_job_list *jobs);


void			init_shell();
void			launch_process(char **p, pid_t pgid, int infile, int outfile, int errfile, int foreground, char **env);

#endif
