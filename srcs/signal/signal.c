/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 13:41:50 by ssoeno            #+#    #+#             */
/*   Updated: 2025/02/08 20:04:40 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/signals.h"
#include "../../includes/utils.h"
#include <sys/wait.h>

volatile sig_atomic_t	g_sig = 0;

void	set_global_sig_variable(int signum)
{
	g_sig = signum;
	rl_replace_line("", 0);
	rl_done = 1;
}

// Sets the handlers for the shell process during input
void	set_idle_sig_handlers(void)
{
	signal(SIGINT, set_global_sig_variable);
	signal(SIGQUIT, SIG_IGN);
}

// Sets the handlers for the parent process during command execution
void	set_parent_sig_handlers(void)
{
	signal(SIGINT, set_exec_handler);
	signal(SIGQUIT, SIG_IGN);
}

/*
Sets the handlers for the child process during command execution.
SIGINT and SIGQUIT are set to their default behavior 
so that the child process can be interrupted.
	Default behavior of SIGINT is to terminate the process.
	Default behavior of SIGQUIT is to terminate the process 
	and generate a core dump.
*/
void	set_child_sig_handlers(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	check_core_dump(int status)
{
	int	sig;

	sig = WTERMSIG(status);
	if (WIFSIGNALED(status))
	{
		if (sig == SIGQUIT)
		{
			ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
		}
	}
}
/*
WIFSIGNALED(status) && (status & 0x80) does NOT work
I don't know why...
*/
