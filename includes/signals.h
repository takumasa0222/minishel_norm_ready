/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 21:49:29 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/04 23:07:19 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H
# define _POSIX_C_SOURCE 200809L
# include <signal.h>

// signal
typedef struct sigaction		t_sig;
extern volatile sig_atomic_t	g_sig;

void	set_idle_handler(void);
void	handler(int signum);
int		check_state(void);
void	ignore_sig(int signum);
void	setup_sigint(void);

// signal_child.c
void	set_exec_child_handler(void);

int	    initialize_rl_event_hook(void);

#endif