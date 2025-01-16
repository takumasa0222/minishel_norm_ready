/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:46 by shokosoeno        #+#    #+#             */
/*   Updated: 2025/01/16 21:37:17 by tamatsuu         ###   ########.fr       */
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

/*
mark as 
syntax error
unexpected error (logicall could not be happened)
system error
permission error
command not found error
Fix
Check
*/

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
	ast_node = parse_cmd(&token_list);//Fix: when there is syntax error, should be throw error and back to readline
	if (!ast_node)
		return ;
	free_token_list(token_list);
	heredoc_handler(ast_node);
	exec_handler(ast_node, ctx);
	if (ctx->cnt)
	{
		wait_children_status(ctx);
		check_core_dump(ctx->last_status);
	}
	free_ast(&ast_node);
}

t_context	*init_ctx(void)
{
	t_context	*ret;

	ret = xmalloc(sizeof(t_context));
	if (!ret)
		d_throw_error("init_ctx", "malloc is failed");
	ret->in_pipe_fd = -1;
	ret->out_pipe_fd = -1;
	ret->pre_in_pipe_fd = -1;
	ret->cnt = 0;
	ret->is_exec_in_child_ps = false;
	ret->is_in_round_bracket = false;
	ret->last_status = 0;
	// ret->stored_stdin = dup(STDIN_FILENO);
	// ret->stored_stdout = dup(STDOUT_FILENO);
	return (ret);
}

t_syntax_err	*init_syntax_error(void)
{
	t_syntax_err	*ret;

	ret = xmalloc(sizeof(t_syntax_err));
	ret->is_err = false;
	ret->err_msg = NULL;
	return (ret);
}

void	clear_ctx(t_context *ctx)
{
	if (!ctx)
		throw_syntax_error("clear_ctx", "ctx is null");//unexpected error
	ctx->in_pipe_fd = -1;
	ctx->out_pipe_fd = -1;
	ctx->pre_in_pipe_fd = -1;
	ctx->cnt = 0;
	ctx->is_exec_in_child_ps = false;
	ctx->is_in_round_bracket = false;
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
