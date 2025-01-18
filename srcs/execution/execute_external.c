/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 02:54:58 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/18 18:16:44 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "../../includes/execute.h"
#include "../../includes/utils.h"
#include "../../includes/environment.h"

static char	*resolve_executable_path(t_node *node, t_map *envp);
static bool	verify_direct_path_or_exit(char *cmd);
static void	execve_errors(char *cmd_path);

int	run_external(t_node *node, t_context *ctx)
{
	char	*cmd_path;

	cmd_path = resolve_executable_path(node, ctx->env);
	if (!cmd_path)
		d_throw_error("exec_cmd", "unexpected: cmd_path is NULL");
	execve(cmd_path, node->cmds, get_environ(ctx->env));
	execve_errors(cmd_path);
	free(cmd_path);
	exit(EXIT_FAILURE);
	return (EXIT_FAILURE);
}

static char	*resolve_executable_path(t_node *node, t_map *envp)
{
	char	*executable_path;
	char	*cmd;
	int		i;

	cmd = node->cmds[0];
	if (ft_strcmp(cmd, ".") == 0 || ft_strcmp(cmd, "..") == 0)
		exit_command_not_found(cmd);
	i = ft_strlen(cmd) - 1;
	while (cmd[i] == '/' && i > 0)
	{
		cmd[i] = '\0';
		i--;
	}
	if (ft_strchr(cmd, '/'))
	{
		verify_direct_path_or_exit(cmd);
		executable_path = cmd;
	}
	else
		executable_path = search_path_env_or_exit(node, envp);
	return (executable_path);
}

static bool	verify_direct_path_or_exit(char *cmd)
{
	struct stat	st;

	if (!cmd)
		d_throw_error("verify_direct_path", "cmd is NULL\n"); // alreadly checked in resolve_executable_path?
	if (access(cmd, F_OK) != 0)
	{
		if (errno == EACCES)
			exit_permission_denied(cmd);
		exit_file_not_found(cmd);
	}
	if (stat(cmd, &st) < 0)
	{
		if (errno == EACCES)
			exit_permission_denied(cmd);
		exit_file_not_found(cmd);
	}
	else if (S_ISDIR(st.st_mode))
		exit_is_directory(cmd);
	if (access(cmd, X_OK) != 0)
	{
		if (errno == EACCES)
			exit_permission_denied(cmd);
		exit_file_not_found(cmd);
	}
	return (true);
}

static void	execve_errors(char *cmd_path)
{
	if (errno == EACCES)
		exit_permission_denied(cmd_path);
	else if (errno == ENOENT)
	{
		ft_putstr_fd(cmd_path, STDERR_FILENO);
		ft_putendl_fd(": bad interpreter", STDERR_FILENO);
		exit(126);
	}
	perror(cmd_path);
	exit(126);
}
