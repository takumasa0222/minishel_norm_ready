/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:31:55 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/11/06 17:27:36 by ssoeno           ###   ########.fr       */
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

// map
typedef struct s_map		t_map;
typedef struct s_item		t_item;

struct s_item {
	char	*name;
	char	*value;
	struct s_item	*next;
};

struct s_map {
	t_item	item_head;
};

t_map	*init_env(char *envp[]);
t_item	*item_new(t_map *map, const char *name);
char 	*item_get_string(t_item *item);
t_map	*map_new(void);
char	*map_get(t_map *map, const char *name);
int		map_put(t_map *map, const char *string, bool allow_empty_value);
int		map_unset(t_map *map, const char *name);
size_t	map_size(t_map *map, bool count_null_value);
void	map_printall(t_map *map);

// map_set.c
int		map_set(t_map *map, const char *name, const char *value);

// env.c
t_map	*init_env(char *envp[]);

// initenv.c
char	*xgetenv(t_map *envmap, const char *name);
char 	**get_environ(t_map *map);

#endif