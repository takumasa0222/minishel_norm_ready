/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 17:56:19 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/27 01:49:03 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"
#include "../includes/execute.h"
#include "../includes/environment.h"
#include "../includes/map.h"

t_context	*init_ctx(char *envp[])
{
	t_context	*ret;

	ret = xmalloc(sizeof(t_context));
	ret->env = NULL;
	ret->in_pipe_fd = -1;
	ret->out_pipe_fd = -1;
	ret->pre_in_pipe_fd = -1;
	ret->cnt = 0;
	ret->is_exec_in_child_ps = false;
	ret->is_in_round_bracket = false;
	ret->last_status = 0;
	ret->stored_stdin = -1;
	ret->stored_stdout = -1;
	ret->heredoc_interrupted = false;
	ret->head_node = NULL;
	ret->env = init_env(envp);
	return (ret);
}

t_syntax_error	*init_syntax_error(void)
{
	t_syntax_error	*ret;

	ret = xmalloc(sizeof(t_syntax_error));
	ret->err_msg = NULL;
	ret->is_error = false;
	return (ret);
}

void	close_stored_fds(t_context *ctx)
{
	if (ctx->stored_stdin != -1 && close(ctx->stored_stdin) == -1)
		d_throw_error("close_stored_fds", "failed to close stored stdin");
	if (ctx->stored_stdout != -1 && close(ctx->stored_stdout) == -1)
		d_throw_error("close_stored_fds", "failed to close stored stdout");
	ctx->stored_stdin = -1;
	ctx->stored_stdout = -1;
}

void	clear_ctx(t_context *ctx)
{
	if (!ctx)
		throw_unexpected_error("clear_ctx", "ctx is null");
	ctx->in_pipe_fd = -1;
	ctx->out_pipe_fd = -1;
	ctx->pre_in_pipe_fd = -1;
	ctx->cnt = 0;
	ctx->is_exec_in_child_ps = false;
	ctx->is_in_round_bracket = false;
	ctx->heredoc_interrupted = false;
	ctx->head_node = NULL;
	backup_std_fds(ctx);
}
