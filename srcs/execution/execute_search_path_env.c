/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_search_path_env.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 02:54:58 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/18 18:09:44 by ssoeno           ###   ########.fr       */
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
static char	*locate_executable_in_dirs(char *cmd, char **directories);

char	*search_path_env_or_exit(t_node *node, t_map *envp)
{
	char	**directories;
	char	*path_env_value;
	char	*executable_path;

	path_env_value = get_resolved_path_env(envp);
	if (!path_env_value)
		exit_file_not_found(node->cmds[0]);
	directories = ft_split(path_env_value, ':');
	free(path_env_value);
	if (!directories)
		d_throw_error("find_executable_path_env_or_exit", "ft_split is failed\n"); // FIX: replace with x_split
	executable_path = locate_executable_in_dirs(node->cmds[0], directories);
	free_wordlist(directories);
	if (!executable_path)
		exit_file_not_found(node->cmds[0]);
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
	copy = ft_strdup(original);
	return (copy);
}

static char	*locate_executable_in_dirs(char *cmd, char **directories)
{
	char	*executable_path;
	char	*dir_with_slash;
	char	*candidate_path;
	int		i;

	executable_path = NULL;
	i = 0;
	while (directories[i])
	{
		dir_with_slash = ft_strjoin(directories[i], "/");
		candidate_path = ft_strjoin(dir_with_slash, cmd);
		free(dir_with_slash);
		if (candidate_path && access(candidate_path, F_OK) == 0)
		{
			executable_path = candidate_path;
			candidate_path = NULL;
			break ;
		}
		else if (errno != ENOENT)
			exit_permission_denied(candidate_path);
		free(candidate_path);
		candidate_path = NULL;
		i++;
	}
	return (executable_path);
}
/*
errno == ENOENT means "file not found" -> continue searching other directories
*/