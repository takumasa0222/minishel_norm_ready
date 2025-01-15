/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_prep.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 02:54:58 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/15 23:23:38 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "../../includes/execute.h"
#include "../../includes/utils.h"


static bool	validate_executable_or_exit(char *cmd);
static char	*find_executable_path_for_cmd(char *cmd, char **directories);
static char	*find_executable_path_env_or_exit(t_node *node, t_map *envp);
static void	command_not_found_error(char *cmd);

char	*resolve_executable_path(t_node *node, t_map *envp)
{
	char	*executable_path;
	char	*cmd;
	int		i;

	cmd = node->cmds[0];
	if (ft_strcmp(cmd, ".") == 0)
		d_throw_error("resolve_executable_path", "filename argument required\n"); // FIX: expect 2
	if (ft_strcmp(cmd, "..") == 0)
		command_not_found_error(cmd);
	i = ft_strlen(cmd) - 1;
	while (cmd[i] == '/' && i > 0)
	{
		cmd[i] = '\0';
		i--;
	}
	if (ft_strchr(cmd, '/'))
	{
		validate_executable_or_exit(cmd);
		executable_path = cmd;
	}
	else
		executable_path = find_executable_path_env_or_exit(node, envp);
	return (executable_path);
}

static bool	validate_executable_or_exit(char *cmd)
{
	if (!cmd)
		d_throw_error("validate_executable_path_or_exit", "cmd is NULL\n");
	if (access(cmd, F_OK) != 0)
		d_throw_error("validate_executable_path_or_exit", "file not found\n"); // FIX: command not found error, exit 127
	else if (access(cmd, X_OK) != 0)
		d_throw_error("validate_executable_path_or_exit", "permission denied\n");
	return (true);
}

static char	*find_executable_path_for_cmd(char *cmd, char **directories)
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
			break ;
		}
		else if (errno != ENOENT)
		{
			perror(candidate_path); // FIX: Error handling
			exit(126);
		}
		free(candidate_path);
		candidate_path = NULL;
		i++;
	}
	return (executable_path);
}
/*
errno == ENOENT means "file not found" -> continue searching other directories
*/

static char	*find_executable_path_env_or_exit(t_node *node, t_map *envp)
{
	char	*path_env_value;
	char	**directories;
	char	*executable_path;
	char	cwd[PATH_MAX];

	path_env_value = map_get(envp, "PATH");
	if (!path_env_value)
		d_throw_error("find_executable_path_env_or_exit", "PATH not set\n"); // FIX: command not found error?
	if (ft_strlen(path_env_value) == 0)
	{
		if (getcwd(cwd, PATH_MAX) == NULL)
		{
			perror("getcwd"); // FIX: Error handling
			exit(EXIT_FAILURE);
		}
		path_env_value = ft_strdup(cwd); // FIX: x_strdup??
		if (!path_env_value)
			d_throw_error("find_executable_path_env_or_exit", "Memory allocation failed\n");
	}
	directories = ft_split(path_env_value, ':');
	if (!directories)
		d_throw_error("find_executable_path_env_or_exit", "ft_split is failed\n"); // FIX: Error handling
	executable_path = find_executable_path_for_cmd(node->cmds[0], directories);
	free_wordlist(directories);
	if (!executable_path)
		d_throw_error("find_executable_path_env_or_exit", "executable path not found\n"); // FIX: exit code should be 127
	return (executable_path);
}

static void	command_not_found_error(char *cmd) // FIX: Error handling
{
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	exit(127);
}
