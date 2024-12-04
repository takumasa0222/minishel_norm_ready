/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:46 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/12/05 04:26:10 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/lexer.h"
#include "../includes/parser.h"

int	main(int argc, char *argv[], char *envp[])
{
	char	*line;

	// test for builtins
	// printf("Testing builtins...\n");
	// printf("Case 1: cd to /tmp and display the directory\n");
	// char *cd_args1[] = {"cd", "/tmp"}; 
	// test_builtin("cd", 2, cd_args1);
	// char *pwd_args1[] = {"pwd"};
	// test_builtin("pwd", 1, pwd_args1);
	// printf("Case 2: cd to non-existent directory\n");
	// char *cd_args2[] = {"cd", "/non-existent-dir"};
	// test_builtin("cd", 2, cd_args2);
	// char *pwd_args2[] = {"pwd"};
	// test_builtin("pwd", 1, pwd_args2);
	// printf("Case 3: Move to the directory one level above the /tmp directory.\n");
	// char *cd_args3[] = {"cd", ".."};
	// test_builtin("cd", 2, cd_args3);
	// char *pwd_args3[] = {"pwd"};
	// test_builtin("pwd", 1, pwd_args3);
	
	// char *exit_args[] = {"exit"};
	// test_builtin("exit", 1, exit_args);
	// printf("Builtins test done.\n");

	rl_outstream = stderr;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
			start_exec(line, envp);
			// TODO: intepret line as a command
		free(line);
	}
	exit(0);
}

void	start_exec(char *line, char **envp)
{
	t_token		*token_list;
	t_node		*ast_node;
	t_context	*ctx;

	token_list = lexer(line);
	ast_node = parse_cmd(&token_list);
	ctx = init_ctx();
	exec_handler(ast_node, envp, ctx);
}

void	exec_handler(t_node *ast_node, char **envp, t_context *ctx)
{
	if (ast_node->kind == ND_CMD)
		exec_cmd(ast_node, envp, ctx);
	//else if (ast_node->kind == ND_SUB_SHELL)
	//	exec_subshell(ast_node, envp, in_fd);
	else if (ast_node->kind == ND_PIPE)
		exec_pipeline(ast_node, envp, ctx);
	//else if (ast_node->kind == ND_OR_OP)
	//	exec_pipe();
	//else if (ast_node->kind == ND_AND_OP)
	//	exec_pipe();
}

exec_pipeline(t_node *node,char **envp, t_context *ctx)
{
	int pid;
	int pfd[2];

	pipe(pfd);
	set_ctx(ctx, pfd);
	exec_handler(node->left, envp, ctx);
	exec_handler(node->right, envp, ctx);
}

child_process(t_node *node)
{
	exec_cmd()
}

//exec_subshell(t_node *left, char **envp, int in_fd)
//{
//	int pid;
	
//	pid = fork();
//	if (pid > 0)
//		exec_handler(left, envp, in_fd);
//	wait();
//}