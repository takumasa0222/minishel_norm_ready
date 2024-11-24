/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:31:55 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/11/24 19:02:48 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _POSIX_C_SOURCE
# define _POSIX_C_SOURCE 200809L
#endif
#ifndef MINISHELL_H
# define MINISHELL_H
# define PATH_MAX 4096

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
void	setup_signal(void);
void	ignore_sig(int signum);
void	setup_sigint(void);

#endif