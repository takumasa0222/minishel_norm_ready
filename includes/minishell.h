/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:31:55 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/12/12 00:47:05 by tamatsuu         ###   ########.fr       */
/*   Updated: 2024/11/25 21:24:15 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PATH_MAX 4096
# define _POSIX_C_SOURCE 200809L

# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <stdbool.h>
# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>

// signal
typedef struct sigaction		t_sig;
extern volatile sig_atomic_t	g_sig;

void	setup_signal(void);
void	handler(int signum);
int		check_state(void);
void	ignore_sig(int signum);
void	setup_sigint(void);

typedef struct s_context {
	pid_t	pids[5000];
	int		in_pipe_fd;
	int		out_pipe_fd;
	int		pre_in_pipe_fd;
	int		cnt;
	bool	is_exec_in_child_ps;
	bool	is_wait_call;
	int		last_status;
}	t_context;

#endif