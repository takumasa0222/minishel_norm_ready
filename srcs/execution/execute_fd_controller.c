/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fd_controller.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:54:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/05 17:47:21 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/utils.h"

//close current pipe_out since no longer use the out fd
//change pipe_in_fd to pre_pipe_in_fd since fd need to use next command input.
//also in_pipe_fd/out_pipe_fd should be set to -1
//if pre_in_pipe_fd is exist close
void	reset_parent_process_fd(t_context *ctx)
{
	printf("DEBUG: reset_parent_process_fd called\n");
	if (ctx->pre_in_pipe_fd != -1 && ctx->pre_in_pipe_fd != STDIN_FILENO)
	{
		if (close(ctx->pre_in_pipe_fd) == -1)
			d_throw_error("reset_parent_process", "fail to cls pre_in_pipe_fd");
	}
	if (ctx->out_pipe_fd != -1 && ctx->out_pipe_fd != STDOUT_FILENO)
		if (close(ctx->out_pipe_fd) == -1)
			d_throw_error("reset_parent_process", "fail to cls out_pipe_fd");
	ctx->pre_in_pipe_fd = ctx->in_pipe_fd;
	ctx->in_pipe_fd = -1;
	ctx->out_pipe_fd = -1;
}

void	set_pipe_fd(int *in_fd, int *out_fd, int *pfd)
{
	if (!in_fd || !out_fd || !pfd)
		d_throw_error("set_pipe_fd", "arg is invalid");
	if (pfd[0] < 0 || pfd[1] < 0)
		d_throw_error("set_pipe_fd", "invalid pipe file descriptors");
	*in_fd = pfd[0];
	*out_fd = pfd[1];
}

void	clear_pid_status(t_context *ctx)
{
	int	i;

	if (!ctx->cnt)
		return ;
	i = 0;
	while (i < ctx->cnt)
	{
		ctx->pids[i] = 0;
		i++;
	}
	ctx->cnt = 0;
}
