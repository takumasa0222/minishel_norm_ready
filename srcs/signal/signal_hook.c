/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 13:41:50 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/05 14:01:49 by ssoeno           ###   ########.fr       */
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
	if (g_sig == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		rl_done = true;
	}
	return (EXIT_SUCCESS);
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
