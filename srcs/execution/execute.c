/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:57:54 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/26 19:16:05 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/signals.h"
#include "../includes/lexer.h"
#include "../includes/execute.h"
#include "../includes/utils.h"
#include "../includes/builtin.h"
#include "../includes/map.h"
#include "../includes/environment.h"
#include "../includes/expand.h"
#include "../includes/redirect.h"

int	exec_handler(t_node *ast_node, t_context *ctx)
{
	if (ast_node->kind == ND_CMD)
		return (exec_cmd_handler(ast_node, ctx));
	else if (ast_node->kind == ND_RND_BRACKET)
		return (exec_round_brackets(ast_node, ctx));
	else if (ast_node->kind == ND_PIPE)
		return (exec_pipe(ast_node, ctx));
	else if (ast_node->kind == ND_OR_OP)
		return (exec_or_node(ast_node, ctx));
	else if (ast_node->kind == ND_AND_OP)
		return (exec_and_node(ast_node, ctx));
	else if (ast_node->kind == ND_REDIRECTS)
		return (exec_redirect(ast_node, ctx));
	else
		return (EXIT_FAILURE);
}

int	exec_redirect(t_node *node, t_context *ctx)
{
	apply_redirects(node, ctx);
	// if (ret != EXIT_SUCCESS)
	// 	close_stored_fds(ctx);
	restore_std_fds(ctx);
	return (ctx->last_status);
}
/*
code rabbit comment: close_stored_fds when apply_redirect fails
ignore for now, FD management will be reviewed in future issues
*/

int	exec_pipe(t_node *node, t_context *ctx)
{
	int	pfd[2];

	if (pipe(pfd))
		d_throw_error("exec_pipeline", "pipe is failed");
	ctx->is_exec_in_child_ps = true;
	set_pipe_fd(&ctx->in_pipe_fd, &ctx->out_pipe_fd, pfd);
	exec_handler(node->left, ctx);
	ctx->out_pipe_fd = STDOUT_FILENO;
	return (exec_handler(node->right, ctx));
}

int	exec_cmd(t_node *node, t_context *ctx)
{
	int	ret;

	ret = 0;
	expand_handler(node, ctx);
	if (node->left)
		set_redirect_fds(node->left, ctx);
	if (ctx->last_status != EXIT_SUCCESS)
		return (ctx->last_status);
	if (is_builtin(node->cmds[0]))
	{
		ret = run_builtin(node, ctx);
		if (ctx->is_exec_in_child_ps)
			exit(ret);
		return (ret);
	}
	else
		return (run_external(node, ctx));
}

int	exec_cmd_handler(t_node *node, t_context *ctx)
{
	if (!ctx->is_exec_in_child_ps && is_builtin(node->cmds[0]))
		return (exec_cmd(node, ctx));
	else
	{
		if (ctx->cnt + 1 >= MAX_PROCESS_COUNT)
			throw_system_error(ERR_MSG_PROCESS_LIMIT, NULL);
		ctx->pids[ctx->cnt] = fork();
		ctx->cnt += 1;
		if (ctx->pids[ctx->cnt -1] == 0)
		{
			set_child_sig_handlers();
			setup_child_process_fd(ctx);
			exec_cmd(node, ctx);
		}
		else if (ctx->pids[ctx->cnt -1] == -1)
			d_throw_error("exec_cmd_handler", "fork is failed");
		else
		{
			set_parent_sig_handlers();
			reset_parent_process_fd(ctx);
			if (node->left && node->left->fd_num != -1)
				close(node->left->fd_num);
		}
	}
	// return (EXIT_SUCCESS);
	return (ctx->last_status);
}
