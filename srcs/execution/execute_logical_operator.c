/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_logical_operator.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:56:20 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/27 00:10:53 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lexer.h"
#include "../includes/execute.h"
#include "../includes/utils.h"

int	exec_or_node(t_node *node, t_context *ctx)
{
	int	status;

	status = 0;
	status = exec_handler(node->left, ctx);
	if (ctx->cnt)
		wait_children_status(ctx);
	if (!ctx->last_status && !status)
	{
		clear_pid_status(ctx);
		// return (EXIT_SUCCESS);
		return (ctx->last_status);
	} 
	// このコメントアウト部分についてたくまささんに確認
	// if (status == 0)
	// 	return (EXIT_SUCCESS);
	else
	{
		if (!ctx->is_in_round_bracket)
			ctx->is_exec_in_child_ps = false;
		clear_pid_status(ctx);
		return (exec_handler(node->right, ctx));
	}
}

int	exec_and_node(t_node *node, t_context *ctx)
{
	int	status;

	status = 0;
	status = exec_handler(node->left, ctx);
	if (ctx->cnt)
		wait_children_status(ctx);
	if (ctx->last_status || status)
	{
		clear_pid_status(ctx);
		if (ctx->last_status)
			return (ctx->last_status);
		return (EXIT_FAILURE);
	}
	else
	{
		if (!ctx->is_in_round_bracket)
			ctx->is_exec_in_child_ps = false;
		clear_pid_status(ctx);
		return (exec_handler(node->right, ctx));
	}
}

int	exec_round_brackets(t_node *node, t_context *ctx)
{
	ctx->is_exec_in_child_ps = true;
	ctx->is_in_round_bracket = true;
	ctx->last_status = exec_handler(node->left, ctx);
	if (ctx->cnt)
		wait_children_status(ctx);
	clear_pid_status(ctx);
	ctx->is_in_round_bracket = false;
	ctx->is_exec_in_child_ps = false;
	return (ctx->last_status);
}
