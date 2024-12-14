/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:57:54 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/15 01:58:06 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lexer.h"
#include "../includes/execute.h"
#include "../includes/utils.h"

int	exec_handler(t_node *ast_node, char **envp, t_context *ctx)
{
	if (ast_node->kind == ND_CMD)
		return (exec_cmd_handler(ast_node, envp, ctx));
	else if (ast_node->kind == ND_SUB_SHELL)
		return (exec_round_brackets(ast_node, envp, ctx));
	else if (ast_node->kind == ND_PIPE)
		return (exec_pipe(ast_node, envp, ctx));
	else if (ast_node->kind == ND_OR_OP)
		return (exec_or_node(ast_node, envp, ctx));
	else if (ast_node->kind == ND_AND_OP)
		return (exec_and_node(ast_node, envp, ctx));
	//else if (ast_node->kind == ND_REDIRECTS)
	//	return (exec_redirects(ast_node, envp, ctx));
	else
		return (EXIT_FAILURE);
}

int	exec_pipe(t_node *node, char **envp, t_context *ctx)
{
	int	pfd[2];

	if (pipe(pfd))
		d_throw_error("exec_pipeline", "pipe is failed");
	ctx->is_exec_in_child_ps = true;
	set_pipe_fd(&ctx->in_pipe_fd, &ctx->out_pipe_fd, pfd);
	exec_handler(node->left, envp, ctx);
	ctx->out_pipe_fd = STDOUT_FILENO;
	return (exec_handler(node->right, envp, ctx));
}

int	exec_cmd(t_node *node, char **envp, t_context *ctx)
{
	//redirect();
	ctx = NULL;
	envp = NULL;
	execvp(node->cmds[0], node->cmds);
	perror("execvp");
	exit(EXIT_FAILURE);
	return (EXIT_FAILURE);
}

int	exec_cmd_handler(t_node *node, char **envp, t_context *ctx)
{
	//if (!ctx->is_exec_in_child_ps && is_builtin(node->cmds[0]))
	if (!ctx->is_exec_in_child_ps && 0)
		return (exec_cmd(node, envp, ctx));
	else
	{
		ctx->pids[ctx->cnt] = fork();
		ctx->cnt += 1;
		if (ctx->pids[ctx->cnt -1] == 0)
		{
			setup_child_process_fd(ctx);
			exec_cmd(node, envp, ctx);
		}
		else if (ctx->pids[ctx->cnt -1] == -1)
			d_throw_error("exec_cmd_handler", "fork is failed");
		else
			reset_parent_process_fd(ctx);
	}
	return (EXIT_SUCCESS);
}
