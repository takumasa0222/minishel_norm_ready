/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:46 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/11/24 05:14:27 by tamatsuu         ###   ########.fr       */
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
	t_token	*token_list;
	t_node	*ast_node;

	token_list = lexer(line);
	ast_node = parse_cmd(&token_list);
	exec_handler(ast_node, envp);
}

void	exec_handler(t_node *ast_node, char **envp, int in_fd)
{
	if (ast_node->kind == ND_CMD)
		exec_cmd();
	else if (ast_node->kind == ND_SUB_SHELL)
		exec_subshell(ast_node, envp, in_fd);
	else if (ast_node->kind == ND_PIPE)
	{
		int pipe_fd[2];
        if (pipe(pipe_fd) == -1) {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }
        pid_t left_pid = fork();
        if (left_pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        if (left_pid == 0) {
            close(pipe_fd[0]);
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[1]);
            exec_handler(ast_node->left, in_fd);
            exit(EXIT_SUCCESS);
        } else {
            close(pipe_fd[1]);
            waitpid(left_pid, NULL, 0);
            if (left_fd != 0) 
				close(left_fd);
            return exec_handler(ast_node->right, pipe_fd[0]);
        }
	}
	else if (ast_node->kind == ND_OR_OP)
		;
	else if (ast_node->kind == ND_AND_OP)
		;
}

exec_pipeline(t_node *node,char **envp, int in_fd)
{
	int pid;
	int pfd[2];
	
	pid = fork();
	pipe(pfd);
	if (pid > 0)
		exec_handler(node->left, envp, in_fd);
	else
		parent_process();
}

child_process(t_node *node)
{
	exec_cmd()
}

exec_subshell(t_node *left, char **envp, int in_fd)
{
	int pid;
	
	pid = fork();
	if (pid > 0)
		exec_handler(left, envp, in_fd);
	wait();
}