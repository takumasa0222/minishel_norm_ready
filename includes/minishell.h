/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:31:55 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/10/27 15:49:38 by ssoeno           ###   ########.fr       */
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
# include "libft.h"

// builtins
typedef struct s_builtin {
	char	*name;
	int		(*f)(int argc, char *argv[]);
}	t_builtin;

void		test_builtin(const char *cmd, int argc, char *argv[]);
t_builtin	*lookup_builtin(char *cmd);
int			builtin_cd(int argc, char *argv[]);
int			builtin_pwd(int argc, char *argv[]);
int			builtin_exit(int argc, char *argv[]);

#endif