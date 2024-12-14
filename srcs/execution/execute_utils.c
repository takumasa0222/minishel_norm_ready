/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:57:36 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/15 02:05:17 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/utils.h"

int	wait_children_status(t_context *ctx)
{
	int	i;
	int	c_status;

	i = -1;
	c_status = EXIT_SUCCESS;
	while (++i < ctx->cnt)
		waitpid(ctx->pids[i], &c_status, 0);
	if (WIFSIGNALED(c_status))
		return (ctx->last_status = WTERMSIG(c_status) + 128, ctx->last_status);
	return (ctx->last_status = WEXITSTATUS(c_status), ctx->last_status);
}

void	setup_child_process_fd(t_context *ctx)
{
	if (ctx->pre_in_pipe_fd != -1)
	{
		dup2(ctx->pre_in_pipe_fd, STDIN_FILENO);
		close(ctx->pre_in_pipe_fd);
	}
	if (ctx->in_pipe_fd != -1)
		close(ctx->in_pipe_fd);
	if (ctx->out_pipe_fd != -1 && ctx->out_pipe_fd != STDOUT_FILENO)
	{
		dup2(ctx->out_pipe_fd, STDOUT_FILENO);
		close(ctx->out_pipe_fd);
	}
}

t_context	*init_ctx(void)
{
	t_context	*ret;

	ret = malloc(sizeof(t_context));
	if (!ret)
		d_throw_error("init_ctx", "malloc is failed");
	ret->in_pipe_fd = -1;
	ret->out_pipe_fd = -1;
	ret->pre_in_pipe_fd = -1;
	ret->cnt = 0;
	ret->is_exec_in_child_ps = false;
	return (ret);
}

bool	is_builtin(char *cmd)
{
	if (!cmd)
		d_throw_error("is_builtin", "command is null");
	if (!ft_strncmp(cmd, EXIT, ft_strlen(EXIT)))
		return (true);
	else if (!ft_strncmp(cmd, CD, ft_strlen(CD)))
		return (true);
	else if (!ft_strncmp(cmd, PWD, ft_strlen(PWD)))
		return (true);
	else if (!ft_strncmp(cmd, UNSET, ft_strlen(UNSET)))
		return (true);
	else if (!ft_strncmp(cmd, ENV, ft_strlen(ENV)))
		return (true);
	else if (!ft_strncmp(cmd, ECHO, ft_strlen(ECHO)))
		return (true);
	else if (!ft_strncmp(cmd, EXPORT, ft_strlen(EXPORT)))
		return (true);
	else
		return (false);
}
