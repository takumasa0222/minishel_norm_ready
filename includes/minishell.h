/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:31:55 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/12/05 04:32:31 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PATH_MAX 4096

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <stdbool.h>
# include "../libft/libft.h"

typedef struct s_context {
	pid_t	*pids;
	int		in_pipe_fd;
	int		out_pipe_fd;
	bool	is_exec_in_child_ps;
}	t_context;

#endif