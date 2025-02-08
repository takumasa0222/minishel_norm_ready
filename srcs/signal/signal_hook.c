/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 13:41:50 by ssoeno            #+#    #+#             */
/*   Updated: 2025/02/08 20:05:05 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/signals.h"
#include "../../includes/utils.h"

bool	is_interactive_mode(void)
{
	return (isatty(STDIN_FILENO));
}

int	sigint_event_hook(void)
{
	return (EXIT_SUCCESS);
}

int	heredoc_sigint_event_hook(void)
{
	if (g_sig == SIGINT)
	{
		rl_done = true;
	}
	return (EXIT_SUCCESS);
}

void	set_exec_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_sig = SIGINT;
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}
/*
g_sig = 0 means no signal is received
SIGINT cancel the current line and update the state of readline
	rl_on_new_line() moves the cursor to the beginning of the next line
	rl_replace_line("", 0) clears the line
	rl_redisplay() displays the prompt
rl_done: set to 1 so that next readline() will return NULL
	and finish the loop
rl_event_hook requires an int return value
*/
