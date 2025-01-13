/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 02:54:58 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/13 17:15:46 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execute.h"
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../../includes/utils.h"
#include "../../includes/environment.h"

static char	*resolve_executable_path(t_node *node, t_map *envp);
static bool	validate_executable_or_exit(char *cmd);
static char	*find_executable_path_for_cmd(char *cmd, char **directories);
static char	*find_executable_path_env_or_exit(t_node *node, t_map *envp);

int	run_external(t_node *node, t_context *ctx)
{
	char	*cmd_path;

	cmd_path = resolve_executable_path(node, ctx->env);
	if (!cmd_path)
		d_throw_error("exec_cmd", "unexpected: cmd_path is NULL");
	execve(cmd_path, node->cmds, get_environ(ctx->env));
	perror("execvp");
	free(cmd_path);
	exit(EXIT_FAILURE);
	return (EXIT_FAILURE);
}

static char	*resolve_executable_path(t_node *node, t_map *envp)
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

static bool	validate_executable_or_exit(char *cmd)
{
	if (!cmd)
		d_throw_error("validate_executable_path_or_exit", "cmd is NULL");
	if (access(cmd, F_OK) != 0)
		d_throw_error("validate_executable_path_or_exit", "file not found");
	if (access(cmd, X_OK) != 0)
		d_throw_error("validate_executable_path_or_exit", "permission denied");
	return (true);
}

static char	*find_executable_path_env_or_exit(t_node *node, t_map *envp)
{
	char	*path_env_value;
	char	**directories;
	char	*executable_path;

	path_env_value = map_get(envp, "PATH");
	if (!path_env_value)
		d_throw_error("find_cmd_from_path", "PATH not set\n");
	directories = ft_split(path_env_value, ':');
	if (!directories)
		d_throw_error("find_cmd_from_path", "ft_split is failed\n");
	executable_path = find_executable_path_for_cmd(node->cmds[0], directories);
	free_wordlist(directories);
	if (!executable_path)
		d_throw_error("find_cmd_from_path", "executable path not found\n");
	return (executable_path);
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
		free(candidate_path);
		candidate_path = NULL;
		i++;
	}
	return (executable_path);
}
