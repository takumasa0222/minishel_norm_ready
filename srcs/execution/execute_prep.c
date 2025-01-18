/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_prep.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 02:54:58 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/18 16:04:00 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execute.h"
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../../includes/utils.h"

bool	validate_executable_or_exit(char *cmd)
{
	if (!cmd)
		d_throw_error("check_executable_path_or_exit", "cmd is NULL");
	if (access(cmd, F_OK) != 0)
		d_throw_error("check_executable_path_or_exit", "file not found");
	if (access(cmd, X_OK) != 0)
		d_throw_error("check_executable_path_or_exit", "permission denied");
	return (true);
}

char	*find_executable_path_for_cmd(char *cmd, char **directories)
{
	char	*executable_path;
	char	*dir_with_slash;
	char	*candidate_path;
	int		i;

	executable_path = NULL;
	i = 0;
	while (directories[i])
	{
		dir_with_slash = ft_strjoin(directories[i], "/");//FIX: xstrjoin should be used.
		candidate_path = ft_strjoin(dir_with_slash, cmd);//FIX: xstrjoin should be used.
		free(dir_with_slash);
		if (candidate_path && access(candidate_path, F_OK) == 0)//CHECK: if path cannot be executable what will be happend?
		{
			executable_path = candidate_path;
			break ;
		}
		free(candidate_path);
		candidate_path = NULL;
		//FIX: dir_with_slash needs to be set NULL
		i++;
	}
	return (executable_path);
}

char	*find_executable_path_env_or_exit(t_node *node, t_map *envp)
{
	char	*path_env_value;
	char	**directories;
	char	*executable_path;

	path_env_value = map_get(envp, "PATH");
	if (!path_env_value)
		d_throw_error("find_cmd_from_path", "PATH not set\n");//FIX: path could be null
	directories = ft_split(path_env_value, ':');
	if (!directories)
		d_throw_error("find_cmd_from_path", "ft_split is failed\n");//FIX: xmalloc should be used;
	executable_path = find_executable_path_for_cmd(node->cmds[0], directories);
	free_wordlist(&directories);
	if (!executable_path)
		d_throw_error("find_cmd_from_path", "executable path not found\n");//command not found error
	return (executable_path);
}

char	*resolve_executable_path(t_node *node, t_map *envp)
{
	char	*executable_path;

	if (ft_strchr(node->cmds[0], '/'))
	{
		validate_executable_or_exit(node->cmds[0]);
		executable_path = node->cmds[0];
	}
	else
		executable_path = find_executable_path_env_or_exit(node, envp);
	return (executable_path);
}
