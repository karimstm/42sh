/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsignal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 11:19:19 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 11:19:20 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

const char			*ft_strsignal(int sig)
{
	static const char *signal_str[] = {
		[SIGHUP] = "Hangup",
		[SIGQUIT] = "Quit",
		"Illegal instruction", "Trace/breakpoint trap", "Aborted",
		"Emulation trap", "Arithmetic exception",
		"Killed", "Bus error", "Segmentation fault",
		"Bad system call", "Broken pipe", "Alarm clock", "Terminated",
		[SIGUSR1] = "User defined signal 1", "User defined signal 2",
		[SIGCHLD] = "Child status changed",
		[SIGWINCH] = "Window size changed",
		[SIGURG] = "Urgent I/O condition",
		[SIGIO] = "I/O possible",
		[SIGSTOP] = "Stopped (signal)",
		[SIGTSTP] = "Stopped (user)",
		[SIGCONT] = "Continued",
		[SIGTTIN] = "Stopped (tty input)",
		[SIGTTOU] = "Stopped (tty output)",
		[SIGVTALRM] = "Virtual timer expired",
		[SIGPROF] = "Profiling timer expired",
		[SIGXCPU] = "CPU time limit exceede",
		[SIGXFSZ] = "File size limit exceeded",
		[SIGINFO] = "Information request"
	};

	return (signal_str[sig]);
}
