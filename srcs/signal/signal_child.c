/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 13:41:50 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/24 19:01:00 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"

/* these functions will be used in child processes */
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