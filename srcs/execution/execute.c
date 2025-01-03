/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:57:54 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/03 14:57:39 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lexer.h"
#include "../includes/execute.h"
#include "../includes/utils.h"
#include "../includes/builtin.h"
#include "../includes/map.h"
#include "../includes/environment.h"
#include "../includes/expand.h"

int	exec_handler(t_node *ast_node, t_map *envp, t_context *ctx)
{
	if (ast_node->kind == ND_CMD)
		return (exec_cmd_handler(ast_node, envp, ctx));
	else if (ast_node->kind == ND_RND_BRACKET)
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

int	exec_pipe(t_node *node, t_map *envp, t_context *ctx)
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


/*

*/
int	exec_cmd(t_node *node, t_map *envp, t_context *ctx)
{
	//signal();
	expand_handler(node, envp);
	//redirect();
	char	*cmd_path;

	if (is_builtin(node->cmds[0]) && 0)
		lookup_builtin(node->cmds[0]);
	else
	{
		ctx = NULL;
		cmd_path = resolve_executable_path(node, envp);
		if (!cmd_path)
			d_throw_error("exec_cmd", "unexpected: cmd_path is NULL");
		execve(cmd_path, node->cmds, get_environ(envp));
		perror("execvp");
		free(cmd_path);
		exit(EXIT_FAILURE);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	exec_cmd_handler(t_node *node, t_map *envp, t_context *ctx)
{
	if (!ctx->is_exec_in_child_ps && is_builtin(node->cmds[0]))
	//if (!ctx->is_exec_in_child_ps && 0)
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
