/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_search_path_env.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 02:54:58 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/31 23:35:17 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "../../includes/execute.h"
#include "../../includes/utils.h"
#include "../../includes/environment.h"

static char	*get_resolved_path_env(t_map *envp);
static char	*locate_executable_in_dirs(char *cmd, char **dirs, bool *flg);

char	*search_path_env_or_exit(t_node *node, t_map *envp)
{
	char	**directories;
	char	*path_env_value;
	char	*executable_path;
	bool	all_permission_denied;

	all_permission_denied = false;
	path_env_value = get_resolved_path_env(envp);
	if (!path_env_value)
		exit_file_not_found(node->cmds[0]);
	directories = x_split(path_env_value, ':');
	free(path_env_value);
	executable_path = locate_executable_in_dirs(node->cmds[0],
			directories, &all_permission_denied);
	free_wordlist(&directories);
	if (!executable_path)
	{
		if (all_permission_denied)
			exit_permission_denied(node->cmds[0]);
		else
			exit_command_not_found(node->cmds[0]);
	}
	return (executable_path);
}

// if PATH = '', replace it with current working directory
static char	*get_resolved_path_env(t_map *envp)
{
	char	*original;
	char	*cwd_ptr;
	char	*copy;

	original = map_get(envp, "PATH");
	if (!original)
		return (NULL);
	if (ft_strlen(original) == 0)
	{
		cwd_ptr = getcwd(NULL, 0);
		if (!cwd_ptr)
		{
			perror("getcwd");
			return (NULL);
		}
		return (cwd_ptr);
	}
	copy = x_strdup(original);
	return (copy);
}

static char	*build_candidate_path(char *dir, char *cmd)
{
	char	*dir_with_slash;
	char	*candidate_path;

	dir_with_slash = x_strjoin(dir, "/");
	candidate_path = x_strjoin(dir_with_slash, cmd);
	free(dir_with_slash);
	return (candidate_path);
}

static char	*check_executable(char *candidate, bool *found_non_exec)
{
	if (candidate && access(candidate, F_OK) == 0)
	{
		if (access(candidate, X_OK) == 0)
			return (candidate);
		else
		{
			*found_non_exec = true;
			return (NULL);
		}
	}
	if (errno != ENOENT)
		exit_permission_denied(candidate);
	return (NULL);
}

static char	*locate_executable_in_dirs(char *cmd, char **dirs, bool *flg)
{
	char	*executable_path;
	char	*candidate_path;
	int		i;

	executable_path = NULL;
	candidate_path = NULL;
	i = 0;
	while (dirs[i])
	{
		candidate_path = build_candidate_path(dirs[i], cmd);
		executable_path = check_executable(candidate_path, flg);
		if (executable_path != NULL)
			break ;
		free(candidate_path);
		candidate_path = NULL;
		i++;
	}
	return (executable_path);
}

// static char	*locate_executable_in_dirs(char *cmd, char **dirs, bool *flg)
// {
// 	char	*executable_path;
// 	char	*candidate_path;
// 	int		i;
// 	executable_path = NULL;
// 	i = 0;
// 	while (dirs[i])
// 	{
// 		candidate_path = build_candidate_path(dirs[i], cmd);
// 		if (access(candidate_path, F_OK) == 0)
// 		{
// 			if (access(candidate_path, X_OK) == 0)
// 			{
// 				executable_path = candidate_path;
// 				break ;
// 			}
// 			else
// 			{
// 				free(candidate_path);
// 				candidate_path = NULL;
// 				*flg = true;
// 				i++;
// 				continue ;
// 			}
// 		}
// 		else if (errno != ENOENT)
// 			exit_permission_denied(candidate_path);
// 		free(candidate_path);
// 		candidate_path = NULL;
// 		i++;
// 	}
// 	return (executable_path);
// }
/*
errno == ENOENT means "file not found"
 -> continue searching other directories
 export PATH=./external_test/a:./external_test/b
*/
