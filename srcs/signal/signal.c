/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 13:41:50 by ssoeno            #+#    #+#             */
/*   Updated: 2024/12/30 21:58:07 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/signals.h"
#include "../../includes/utils.h"

volatile sig_atomic_t	g_sig = 0;

void	handler(int signum)
{
	g_sig = signum;
}

void	ignore_sig(int signum)
{
	t_sig	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	if (sigaction(signum, &sa, NULL) < 0)
		d_throw_error("ignore_sig", "sigaction failed");
}

void	setup_sigint(void)
{
	t_sig	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if (sigaction(SIGINT, &sa, NULL) < 0)
		d_throw_error("setup_sigint", "sigaction failed");
}
/*
call hander when SIGINT is received
sa_flag is set to 0 unless you want to use specific flags
like SA_RESTART or SA_SNOCLDSTOP
*/

int	reset_line_on_sigint(void)
{
	if (g_sig == 0)
		return (0);
	else if (g_sig == SIGINT)
	{
		g_sig = 0;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		rl_done = 1;
		return (0);
	}
	return (0);
}
/*
sig = 0 means no signal is received
SIGINT cancel the current line and update the state of readline
	rl_on_new_line() moves the cursor to the beginning of the next line
	rl_replace_line("", 0) clears the line
	rl_redisplay() displays the prompt
	if rl_on_new_line() is not called, 
	the prompt will be displayed on the same line
rl_done is a global variable of readline 
	when rl_done is set to 1, next readline() will return NULL
	and finish the loop
rl_event_hook requires an int return value
*/

void	setup_signal(void)
{
	rl_outstream = stderr;
	if (isatty(STDIN_FILENO))
		rl_event_hook = reset_line_on_sigint;
	ignore_sig(SIGQUIT);
	setup_sigint();
}
/*
- Ignore SIGQUIT (Ctrl+\) to prevent the shell from exiting.
- isatty() returns 1 if the fd is associated with a terminal
- rl_event_hook is a function pointer that is called before readline() returns
- Non-printable characters (such as ^C) are displayed
	in the bash terminal at 42 tokyo.
*/
