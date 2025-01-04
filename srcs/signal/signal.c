/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 13:41:50 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/04 23:03:54 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/signals.h"
#include "../../includes/utils.h"
#include <sys/time.h>

volatile sig_atomic_t	g_sig = 0;

void 	idle_handler(int signum)
{
	(void)signum;
	// static int count = 0;
	// struct timeval tv;
	// gettimeofday(&tv, NULL);
	// fprintf(stderr,
    //     "DEBUG: idle_handler called with signum=%d count=%d time=%ld.%06ld\n",
    //     signum, count, (long)tv.tv_sec, (long)tv.tv_usec);
	g_sig = SIGINT;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	rl_done = 1;
}

void	set_idle_handler(void)
{
	t_sig 		sa;

	sa.sa_handler = idle_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
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
/*
sig = 0 means no signal is received
SIGINT cancel the current line and update the state of readline
	rl_on_new_line() moves the cursor to the beginning of the next line
	rl_replace_line("", 0) clears the line
	rl_redisplay() displays the prompt
rl_done: set to 1 so that next readline() will return NULL
	and finish the loop
rl_event_hook requires an int return value
*/

int	initialize_rl_event_hook(void)
{
	return 0;
}