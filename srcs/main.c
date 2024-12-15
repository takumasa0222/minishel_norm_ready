/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:46 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/12/15 02:06:27 by tamatsuu         ###   ########.fr       */
/*   Updated: 2024/11/24 14:12:36 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/lexer.h"
#include "../includes/parser.h"
#include "../includes/execute.h"
#include "../includes/utils.h"

int	main(int argc, char *argv[], char *envp[])
{
	char	*line;
	rl_outstream = stderr;
	setup_signal();
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
      // start_exec(line, envp);
		free(line);
	}
	exit(0);
}

//void	start_exec(char *line, char **envp)
//{
//	t_token		*token_list;
//	t_node		*ast_node;

//	token_list = lexer(line);
//	ast_node = parse_cmd(&token_list);
//	ctx = init_ctx();
//	exec_handler(ast_node, envp, ctx);
//	if (ctx->cnt)
//		wait_children_status(ctx);
//}