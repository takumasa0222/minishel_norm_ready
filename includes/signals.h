/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 21:49:29 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/30 21:51:47 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H
# define _POSIX_C_SOURCE 200809L


# include <signal.h>
// signal
typedef struct sigaction		t_sig;
extern volatile sig_atomic_t	g_sig;

void	setup_signal(void);
void	handler(int signum);
int		check_state(void);
void	ignore_sig(int signum);
void	setup_sigint(void);
#endif