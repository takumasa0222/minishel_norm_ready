/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:46 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/12/08 19:22:07 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/lexer.h"
#include "../includes/parser.h"
#include "../includes/execute.h"
#include "../includes/utils.h"

//int	main(int argc, char *argv[], char *envp[])
//{
//	char	*line;

//	// test for builtins
//	// printf("Testing builtins...\n");
//	// printf("Case 1: cd to /tmp and display the directory\n");
//	// char *cd_args1[] = {"cd", "/tmp"}; 
//	// test_builtin("cd", 2, cd_args1);
//	// char *pwd_args1[] = {"pwd"};
//	// test_builtin("pwd", 1, pwd_args1);
//	// printf("Case 2: cd to non-existent directory\n");
//	// char *cd_args2[] = {"cd", "/non-existent-dir"};
//	// test_builtin("cd", 2, cd_args2);
//	// char *pwd_args2[] = {"pwd"};
//	// test_builtin("pwd", 1, pwd_args2);
//	// printf("Case 3: Move to the directory one level above the /tmp directory.\n");
//	// char *cd_args3[] = {"cd", ".."};
//	// test_builtin("cd", 2, cd_args3);
//	// char *pwd_args3[] = {"pwd"};
//	// test_builtin("pwd", 1, pwd_args3);
	
//	// char *exit_args[] = {"exit"};
//	// test_builtin("exit", 1, exit_args);
//	// printf("Builtins test done.\n");

//	rl_outstream = stderr;
//	while (1)
//	{
//		line = readline("minishell$ ");
//		if (line == NULL)
//			break ;
//		if (*line)
//			add_history(line);
//			start_exec(line, envp);
//			// TODO: intepret line as a command
//		free(line);
//	}
//	exit(0);
//}

//void	start_exec(char *line, char **envp)
//{
//	t_token		*token_list;
//	t_node		*ast_node;

//	token_list = lexer(line);
//	ast_node = parse_cmd(&token_list);
//	ctx = init_ctx();
//	exec_handler(ast_node, envp, ctx);

	//return (WEXITSTATUS(c_status));
//}



void	exec_handler(t_node *ast_node, char **envp, t_context *ctx)
{

	if (ast_node->kind == ND_CMD)
		exec_cmd_handler(ast_node, envp, ctx);
	//else if (ast_node->kind == ND_SUB_SHELL)
	//	exec_subshell(ast_node, envp, in_fd);
	else if (ast_node->kind == ND_PIPE)
		exec_pipeline(ast_node, envp, ctx);
	//else if (ast_node->kind == ND_OR_OP)
	//	exec_pipe();
	//else if (ast_node->kind == ND_AND_OP)
	//	exec_pipe();

}

void	exec_pipeline(t_node *node, char **envp, t_context *ctx)
{
	int	pfd[2];

	if (pipe(pfd))
		d_throw_error("exec_pipeline", "pipe is failed");
	ctx->is_exec_in_child_ps = true;
	set_pipe_fd(&ctx->in_pipe_fd, &ctx->out_pipe_fd, pfd);
	exec_handler(node->left, envp, ctx);
	exec_handler(node->right, envp, ctx);
}

void	exec_child_process(t_node *node, char **envp, t_context *ctx)
{
	// read data from pre_pipe_in
	if (ctx->pre_in_pipe_fd != -1)
		dup2(ctx->pre_in_pipe_fd, STDIN_FILENO);
	close(ctx->in_pipe_fd);
	close(ctx->pre_in_pipe_fd);
	// write data to current pipe
	if (ctx->out_pipe_fd != -1)
		dup2(ctx->out_pipe_fd, STDOUT_FILENO);
	exec_cmd(node, envp, ctx);
}

void	exec_cmd(t_node *node, char **envp, t_context *ctx)
{
	//redirect();
	ctx = init_ctx();
	execve(node->cmds[0], node->cmds, envp);
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

void	exec_cmd_handler(t_node *node, char **envp, t_context *ctx)
{
	if (ctx->is_exec_in_child_ps)
	{
		ctx->pids[ctx->cnt] = fork();
		ctx->cnt += 1;
		if (ctx->pids[ctx->cnt -1] > 0)
		{
			exec_child_process(node, envp, ctx);
		}
		else 
		{
			parent_process_fd_reset(ctx);
		}
	}
}

//close current pipe_out since no longer use the out fd
//change pipe_in_fd to pre_pipe_in_fd since fd need to use next command input.
//also in_pipe_fd/out_pipe_fd should be set to -1
//if pre_in_pipe_fd is exist close
void	parent_process_fd_reset(t_context *ctx)
{
	if (ctx->pre_in_pipe_fd != -1)
		close(ctx->pre_in_pipe_fd);
	if (ctx->out_pipe_fd != -1)
		close(ctx->out_pipe_fd);
	ctx->pre_in_pipe_fd = ctx->in_pipe_fd;
	ctx->in_pipe_fd = -1;
	ctx->out_pipe_fd = -1;
}

void	set_pipe_fd(int *in_fd, int *out_fd, int *pfd)
{
	if (!in_fd || !out_fd || !pfd)
		d_throw_error("set_pipe_fd", "arg is invalid");
	*in_fd = pfd[0];
	*out_fd = pfd[1];
}

//exec_subshell(t_node *left, char **envp, int in_fd)
//{
//	int pid;
	
//	pid = fork();
//	if (pid > 0)
//		exec_handler(left, envp, in_fd);
//	wait();
//}