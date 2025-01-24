/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:54:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/23 23:36:28 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/utils.h"
#include "../includes/builtin.h"

void	backup_std_fds(t_context *ctx);
void	restore_std_fds(t_context *ctx);
int		exec_builtin(char *cmd, char **argv, t_context *ctx);

int	run_builtin(t_node *node, t_context *ctx)
{
	int	ret;

	ret = exec_builtin(node->cmds[0], node->cmds, ctx);
	if (ctx->is_exec_in_child_ps)
		exit(ret);
	return (ret);
}

void	backup_std_fds(t_context *ctx)
{
	if (ctx->stored_stdin >= 0)
		close(ctx->stored_stdin);
	if (ctx->stored_stdout >= 0)
		close(ctx->stored_stdout);
	ctx->stored_stdin = dup(STDIN_FILENO);
	ctx->stored_stdout = dup(STDOUT_FILENO);
	if (ctx->stored_stdin < 0 || ctx->stored_stdout < 0)
		d_throw_error("backup_std_fds", "dup failed");
}

void	restore_std_fds(t_context *ctx)
{
	if (ctx->stored_stdin >= 0)
	{
		if (dup2(ctx->stored_stdin, STDIN_FILENO) < 0)
			d_throw_error("restore_std_fds", "dup2 failed");
	}
	if (ctx->stored_stdout >= 0)
	{
		if (dup2(ctx->stored_stdout, STDOUT_FILENO) < 0)
			d_throw_error("restore_std_fds", "dup2 failed");
	}
	// close(ctx->stored_stdin);
	// close(ctx->stored_stdout);
	// ctx->stored_stdin = -1;
	// ctx->stored_stdout = -1;
}

int	exec_builtin(char *cmd, char **argv, t_context *ctx)
{
	t_builtin	*builtin;
	int			arr_size;

	arr_size = 0;
	// backup_std_fds(ctx);
	builtin = lookup_builtin(cmd);
	if (!builtin)
	{
		ft_putendl_fd("exec_builtin: builtin not found\n", STDERR_FILENO);
		ctx->last_status = EXIT_FAILURE;
		return (ctx->last_status);
	}
	while (argv[arr_size])
		arr_size++;
	ctx->last_status = builtin->f(arr_size, argv, ctx);
	if (ctx->last_status != EXIT_SUCCESS)
	{
		ft_putendl_fd("exec_builtin failed\n", STDERR_FILENO);
		return (ctx->last_status);
	}
	restore_std_fds(ctx);
	return (ctx->last_status);
}
