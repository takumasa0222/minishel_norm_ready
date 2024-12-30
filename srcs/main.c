/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:46 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/12/30 22:03:22 by tamatsuu         ###   ########.fr       */
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

int	main(int argc, char *argv[], char *envp[])
{
	char	*line;
	//t_token	*token_list;
	t_map	*env;
	//t_node	*ast;

	if (argc >= 2)
		ft_putendl_fd("command line arguments will be ignored", STDERR_FILENO);
	(void)argv;
	rl_outstream = stderr;
	setup_signal();
	env = init_env(envp);
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
		{
			add_history(line);
			start_exec(line, env);
			//token_list = lexer(line);
			//ast = parse_cmd(&token_list);
			//free_node(ast);
			//ast = NULL;
			//if (token_list)
			//	free_token_list(token_list);
			//token_list = NULL;
		}
		free(line);
		line = NULL;
	}
	exit(0);
}

void	start_exec(char *line, t_map *envp)
{
	t_token		*token_list;
	t_node		*ast_node;
	t_context	*ctx;

	token_list = lexer(line);
	ast_node = parse_cmd(&token_list);
	ctx = init_ctx();
	exec_handler(ast_node, envp, ctx);
	if (ctx->cnt)
		wait_children_status(ctx);
	free_token_list(token_list);
	free_ast(&ast_node);
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
