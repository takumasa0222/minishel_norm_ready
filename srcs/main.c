/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:46 by shokosoeno        #+#    #+#             */
/*   Updated: 2025/01/23 03:24:11 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"
#include "../includes/lexer.h"
#include "../includes/parser.h"
#include "../includes/execute.h"
#include "../includes/utils.h"
#include "../includes/map.h"
#include "../includes/environment.h"
#include "../includes/signals.h"
#include "../includes/heredoc.h"

int	main(int argc, char *argv[], char *envp[])
{
	char		*line;
	t_context	*ctx;

	if (argc >= 2)
		ft_putendl_fd("command line arguments will be ignored", STDERR_FILENO);
	(void)argv;
	ctx = init_ctx();
	rl_outstream = stderr;
	if (isatty(STDIN_FILENO))
		rl_event_hook = sigint_event_hook;
	ctx->env = init_env(envp);
	while (1)
	{
		set_idle_sig_handlers();
		if (ctx->heredoc_interrupted == true)
		{
			ctx->heredoc_interrupted = false;
			g_sig = 0;
			ctx->last_status = 130;
			if (isatty(STDIN_FILENO))
			{
				rl_event_hook = sigint_event_hook;
				continue ;
			} else {
				break;
			}
		}
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (g_sig != 0)
		{
			ctx->last_status = g_sig + 128;
			g_sig = 0;
		}
		if (*line && !is_blanc_line(line))
		{
			add_history(line);
			start_exec(line, ctx);
		}
		free(line);
		line = NULL;
	}
	return (ctx->last_status);
}

void	start_exec(char *line, t_context *ctx)
{
	t_token			*token_list;
	t_node			*ast_node;
	t_syntax_error	*syntx_err;

	syntx_err = init_syntax_error();
	clear_ctx(ctx);
	token_list = lexer_handler(line, syntx_err, ctx);
	if (!token_list)
		return ;
	ast_node = parse_cmd_handler(&token_list, syntx_err, ctx);
	if (!ast_node)
		return ;
	heredoc_handler(ast_node, ctx);
	exec_handler(ast_node, ctx);
	// restore fds?
	if (ctx->cnt)
	{
		wait_children_status(ctx);
		check_core_dump(ctx->last_status);
	}
	free_ast(&ast_node);
	close_stored_fds(ctx);
}

void close_stored_fds(t_context *ctx)
{
	if (ctx->stored_stdin != -1 && close(ctx->stored_stdin) == -1)
		d_throw_error("close_stored_fds", "failed to close stored stdin");
	if (ctx->stored_stdout != -1 && close(ctx->stored_stdout) == -1)
		d_throw_error("close_stored_fds", "failed to close stored stdout");
	ctx->stored_stdin = -1;
	ctx->stored_stdout = -1;
}

t_context	*init_ctx(void)
{
	t_context	*ret;

	ret = xmalloc(sizeof(t_context));
	if (!ret)
		throw_unexpected_error("init_ctx", "malloc is failed");
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
	return (ret);
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
	backup_std_fds(ctx);
}
bool	is_blanc_line(char *line)
{
	size_t	i;

	if (!line)
		return (true);
	i = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t')
			i++;
		else
			return (false);
	}
	return (true);
}

t_syntax_error	*init_syntax_error(void)
{
	t_syntax_error	*ret;

	ret = xmalloc(sizeof(t_syntax_error));
	ret->err_msg = NULL;
	ret->is_error = false;
	return (ret);
}
