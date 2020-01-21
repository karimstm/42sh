# include "shell.h"



int			bg_usage(char c)
{
	if (c)
		ft_printf_fd(2, "42sh: bg: -%c: invalid option\n", c);
	ft_printf_fd(2, "fg: usage: bg [job_spec]\n");
	return (-1);
}

int			bg_error(char *job)
{
	ft_printf_fd(2, "bash: bg: %s: no such job\n", job);
	return (-1);
}

int			bg_get_job_number(char **args)
{
	int		job_number;
	char	*tmp;

	job_number = 0;
	if (args != NULL)
	{
		if (args[0])
		{
			tmp = args[0];
			if (*tmp == '%')
				tmp++;
			if (*tmp)
			{
				if (*tmp == '+')
					return ('+');
				else if (*tmp == '-' && tmp[1] == '0')
					return ('-');
				else if (is_number(tmp))
					return (ft_atoi(tmp));
			}
			if (args[0][0] == '-' && args[0][1])
				return (bg_usage(args[0][1]));
			else
				return (bg_error(args[0]));
		}
	}
	return (job_number);
}


void		ft_bg(char **args, t_list **env)
{
	t_job		*current;
	t_job_list	*list;
	int			job_number;

	(void)env;
	list = get_job_list(NULL);
	if ((job_number = bg_get_job_number(args)) == -1)
		return ;
	if ((current = get_job(list, job_number)))
		continue_job(list, current, 0);
}
