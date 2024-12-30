/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_prep.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 02:54:58 by ssoeno            #+#    #+#             */
/*   Updated: 2024/12/31 04:23:46 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execute.h"
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../../includes/utils.h"

bool is_executable_or_exit(char *cmd)
{
	if (!cmd)
		d_throw_error("check_executable_path_or_exit", "cmd is NULL");
	if (access(cmd, F_OK) != 0)
		d_throw_error("check_executable_path_or_exit", "file not found");
	if (access(cmd, X_OK) != 0)
		d_throw_error("check_executable_path_or_exit", "permission denied");
	return (true);
}

char	*find_cmd_in_env(char *cmd, t_map *envp)
{
	char	*path_env_value;
	char	**directories;
	char	*candidate_path;
	char	*executable_path;
	int		i;
	char	*dir_with_slash;

	path_env_value = map_get(envp, "PATH");
	if (!path_env_value)
		d_throw_error("find_cmd_from_path", "PATH not set");
	directories = ft_split(path_env_value, ':');
	if (!directories)
		d_throw_error("find_cmd_from_path", "ft_split is failed");
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
			break;
		}
		free(executable_path);
		i++;
	}
	free_wordlist(directories);
	if (!executable_path)
		d_throw_error("find_cmd_from_path", "executable not found");
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
		executable_path = find_cmd_from_env_or_exit(node, envp);
	return (executable_path);
}