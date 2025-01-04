/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 13:41:50 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/04 21:55:05 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/signals.h"
#include "../../includes/minishell.h"
#include "../../includes/utils.h"

/*
Shell process (parent):
	SIGINT and SIGQUIT doesn’t terminate the shell
Child process:
	Gets signals in the “normal” Unix way: 
	pressing Ctrl+C kills or interrupts the child.
	Pressing Ctrl+\ sends SIGQUIT, 
	which might produce a core dump by default, etc.
*/
// void	reset_sig(int signum)
// {
// 	t_sig	sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;
// 	sa.sa_handler = SIG_DFL;
// 	if (sigaction(signum, &sa, NULL) < 0)
// 		d_throw_error("reset_sig", "sigaction failed");
// }
// void	reset_signal(void)
// {
// 	reset_sig(SIGQUIT);
// 	reset_sig(SIGINT);
// }
void	set_exec_child_handler(void)
{
	struct sigaction	sa;
	
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	if (sigaction(SIGQUIT, &sa, NULL) < 0)
		d_throw_error("set_exec_child_handler", "sigaction failed");
	if (sigaction(SIGINT, &sa, NULL) < 0)
		d_throw_error("set_exec_child_handler", "sigaction failed");
}
// set defalut for SIGQUIT and SIGINT
// SIGIGN also works for SIGQUIT