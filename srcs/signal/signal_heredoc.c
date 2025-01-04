/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 13:41:50 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/04 15:48:34 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/signals.h"
#include "../../includes/minishell.h"
#include "../../includes/utils.h"

/*
The heredoc reading might happen in child processes??

1. Ignore SIGQUIT in the parent's heredoc parser
	so it doesn't prduce a core dump or kill the child
2. Restore the default behavior for SIGINT in the child's heredoc parser
	SIG_DFL means "go back to the default behavior"
*/
// void	set_heredoc_handler(void)
// {
// 	struct sigaction	sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;
// 	// ignore SIGQUIT
// 	sa.sa_handler = SIG_IGN;
// 	sigaction(SIGQUIT, &sa, NULL);
// 	// restore default behavior for SIGINT
// 	sa.sa_handler = SIG_DFL;
// 	sigaction(SIGINT, &sa, NULL);
// }
