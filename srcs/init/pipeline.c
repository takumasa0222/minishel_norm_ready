/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 23:52:40 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 21:16:59 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../includes/lexer.h"
#include "../includes/parser.h"
#include "../includes/execute.h"
#include "../includes/utils.h"
#include "../includes/map.h"
#include "../includes/environment.h"
#include "../includes/signals.h"
#include "../includes/heredoc.h"

void	start_pipeline(char *line, t_context *ctx)
{
	t_node			*ast_node;

	clear_ctx(ctx);
	ast_node = analyze_pipeline(line, ctx);
	if (!ast_node)
		return ;
	ctx->head_node = &ast_node;
	exec_handler(ast_node, ctx);
	if (ctx->cnt)
	{
		wait_children_status(ctx);
		check_core_dump(ctx->last_status);
	}
	free_ast(&ast_node);
	close_stored_fds(ctx);
	ctx->head_node = NULL;
}

t_node	*analyze_pipeline(char *line, t_context *ctx)
{
	t_token			*token_list;
	t_syntax_error	*syntx_err;
	t_node			*ast_node;
	bool			is_sigint;

	syntx_err = init_syntax_error();
	token_list = lexer_handler(line, syntx_err, ctx);
	if (!token_list)
		return (NULL);
	ast_node = parse_cmd_handler(&token_list, syntx_err, ctx);
	if (!ast_node)
		return (NULL);
	is_sigint = false;
	heredoc_handler(ast_node, ctx, &is_sigint);
	if (is_sigint)
	{
		free_ast(&ast_node);
		close_stored_fds(ctx);
		ctx->head_node = NULL;
		return (NULL);
	}
	return (ast_node);
}
