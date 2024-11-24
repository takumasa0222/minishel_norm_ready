/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 13:41:50 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/24 17:40:20 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"

volatile sig_atomic_t	g_sig = 0;

// 130 は SIGINT の終了コード

void	handler(int signum)
{
	g_sig = signum;
}

void	reset_sig(int signum)
{
	t_sig	sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	if (sigaction(signum, &sa, NULL) < 0)
		d_throw_error("reset_sig", "sigaction failed");
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
sa_flag is set to 0 unless you want to use specific flags
like SA_RESTART or SA_SNOCLDSTOP
*/

#include <stdio.h>
#include <readline/readline.h>
int	check_state(void)
{
	if (g_sig == 0) // no signal received
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
SIGINT cancel the current line and update the state of readline
    rl_on_new_line() moves the cursor to the beginning of the next line
    rl_replace_line("", 0) clears the line
    rl_redisplay() displays the prompt
    if rl_on_new_line() is not called, 
    the prompt will be displayed on the same line
rl_done is a global variable of readline 
when rl_done is set to 1, next readline() will return NULL
and finish the loop
*/

void	setup_signal(void)
{
	// extern int	_rl_echo_control_chars;
	// _rl_echo_control_chars = 0;
	rl_outstream = stderr;
	if (isatty(STDIN_FILENO))
		rl_event_hook = check_state;
	ignore_sig(SIGQUIT);
	setup_sigint();
}
/*
_rl_echo_control_chars = 0; readline は制御文字を非表示にする。
readline の rl_hook を使って入力後の処理を変更することも可能
例えば、Ctrl+C を押しても ^C のような制御文字が表示されない
ignore SIGQUIT (Ctrl+\) to prevent the shell from exiting
*/

void	reset_signal(void)
{
	reset_sig(SIGQUIT);
	reset_sig(SIGINT);
}
