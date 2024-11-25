/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 20:58:10 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/25 21:25:30 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H
# include <stdbool.h>
# include <stdlib.h>

// map
typedef struct s_map		t_map;
typedef struct s_item		t_item;

struct s_item {
	char			*name;
	char			*value;
	struct s_item	*next;
};

struct s_map {
	t_item	item_head;
};
/* t_map is a dummy head of the linked list of items
which is not counted in the size of the map
item_head is not a pointer so its memory is automatically 
allocated when t_map is created
*/

// structure for builtins
typedef struct s_builtin {
	char	*name;
	int		(*f)(int argc, char *argv[], t_map *envmap);
}	t_builtin;

t_builtin	*lookup_builtin(char *cmd);

// init_env.c
t_map		*init_env(char *envp[]);
char		*xgetenv(t_map *envmap, const char *name);
char		**get_environ(t_map *map);

// item_utils.c
t_item		*item_new(char *name, char *value);
char		*item_get_string(t_item *item);
int			ft_strcmp_for_map(const char *s1, const char *s2);

// map_get.c
char		*map_get(t_map *map, const char *name);

// map_new.c
t_map		*map_new(void);

// map_put.c
int			map_put(t_map *map, const char *string, bool allow_empty_value);

// map_set.c
int			map_set(t_map *map, const char *name, const char *value);
bool		is_identifier(const char *s);

// map_size.c
size_t		map_size(t_map *map, bool count_null_value);

// map_unset.c
int			map_unset(t_map *map, const char *name);

// builtin_cd.c
int			builtin_cd(int argc, char *argv[], t_map *envmap);

// builtin_cd_utils.c
bool		consume_path(char **rest, char *path, char *elem);
void		delete_last_elem(char *path);
void		append_path_elem(char *dst, char **rest, char *src);
char		*resolve_pwd(char *pwd_before_chdir, char *path);

// builtin pwd, exit, unset, env, echo
int			builtin_pwd(int argc, char *argv[], t_map *envmap);
int			builtin_exit(int argc, char *argv[], t_map *envmap);
int			builtin_unset(int argc, char *argv[], t_map *envmap);
int			builtin_env(int argc, char *argv[], t_map *envmap);
int			builtin_echo(int argc, char *argv[], t_map *envmap);

// builtin_export.c
void		print_allenv(t_map *envmap);
int			builtin_export(int argc, char **argv, t_map *envmap);

#endif