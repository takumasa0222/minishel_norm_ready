/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:46 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/12/12 04:18:29 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/lexer.h"
#include "../includes/parser.h"
#include "../includes/execute.h"
#include "../includes/utils.h"
#include <errno.h>

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
	//while (++i <= ctx->cnt)
	//	waitpid(ctx->pids[i], &c_status, 0);
	//if (WIFSIGNALED(c_status))
	//	return (WTERMSIG(c_status) + 128);
//	return (WEXITSTATUS(c_status));
//}



int	exec_handler(t_node *ast_node, char **envp, t_context *ctx)
{
	if (ast_node->kind == ND_CMD)
		return (exec_cmd_handler(ast_node, envp, ctx));
	//else if (ast_node->kind == ND_SUB_SHELL)
	//	exec_subshell(ast_node, envp, in_fd);
	else if (ast_node->kind == ND_PIPE)
	{
		return exec_pipe(ast_node, envp, ctx);
		//return (wait_child_status(ctx));
	}
	else if (ast_node->kind == ND_OR_OP)
		return (exec_or_node(ast_node, envp, ctx));
	//else if (ast_node->kind == ND_AND_OP)
	//	exec_pipe();
	return (EXIT_FAILURE);
}

int	exec_pipe(t_node *node, char **envp, t_context *ctx)
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

//今の所、パイプで最後に実行されるものがわからない限り、毎回 waitpid を読んで処理を止めるのは無理そう
//最終のコマンドであった場合に、こちらのコマンドを呼んでという実装ができればうまく行く。
//無論、全体に対して呼ぶことで、パイプのみのケースであればうまく行くが、他のものとの組み合わせのときにうまく動作しない。
//理由としては、個別に呼び出すと、並列的に実行されるべきコマンドが、順次実行となってしまうから。
int	wait_childlen_status(t_context *ctx)
{
	int	i;
	int	c_status;

	//if (ctx->is_wait_call)
	//	return (ctx->last_status);
	i = -1;
	c_status = 0;
	while (++i < ctx->cnt)
		waitpid(ctx->pids[i], &c_status, 0);
	//ctx->is_wait_call = true;
	if (WIFSIGNALED(c_status))
		return (ctx->last_status = WTERMSIG(c_status) + 128, ctx->last_status);
	return (ctx->last_status = WEXITSTATUS(c_status), ctx->last_status);
}



//int	wait_any_child(t_context *ctx)
//{
//	int	c_status;
//	int	ret;

//	ret = 0;
//	//if (ctx->is_wait_call)
//	//	return (ctx->last_status);
//	c_status = 0;
//	ret = waitpid(-1, &c_status, WNOHANG);
//	//ctx->is_wait_call = true;
//	if (WIFSIGNALED(c_status))
//		return (ctx->last_status = WTERMSIG(c_status) + 128, ctx->last_status);
//	return (ctx->last_status = WEXITSTATUS(c_status), ctx->last_status);
//}

void	exec_child_process(t_node *node, char **envp, t_context *ctx)
{
	// read data from pre_pipe_in
	if (ctx->pre_in_pipe_fd != -1)
	{
		dup2(ctx->pre_in_pipe_fd, STDIN_FILENO);
		close(ctx->pre_in_pipe_fd);
	}
	// close because unused.
	if (ctx->in_pipe_fd != -1)
		close(ctx->in_pipe_fd);
	// write data to current pipe
	if (ctx->out_pipe_fd != -1 && ctx->out_pipe_fd != STDOUT_FILENO)
	{
		dup2(ctx->out_pipe_fd, STDOUT_FILENO);
		close(ctx->out_pipe_fd);
	}
	exec_cmd(node, envp, ctx);
}

int	exec_cmd(t_node *node, char **envp, t_context *ctx)
{
	//redirect();
	ctx = NULL;
	envp = NULL;
	execvp(node->cmds[0], node->cmds);
	perror("");
	exit(EXIT_FAILURE);
	//unreachable
	return (EXIT_FAILURE);
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

int	exec_cmd_handler(t_node *node, char **envp, t_context *ctx)
{
	//if (ctx->is_exec_in_child_ps || !is_builtin())
	if (ctx->is_exec_in_child_ps || 1)
	{
		ctx->pids[ctx->cnt] = fork();
		ctx->cnt += 1;
		if (ctx->pids[ctx->cnt -1] == 0)
			exec_child_process(node, envp, ctx);
		else if (ctx->pids[ctx->cnt -1] == -1)
			d_throw_error("exec_cmd_handler", "fork is failed");
		else
		{
			if (ctx->out_pipe_fd == STDOUT_FILENO)
				return (wait_childlen_status(ctx));
			parent_process_fd_reset(ctx);
		}
	}
	else
		return (exec_cmd(node, envp, ctx));
	// temporaly adding
	return	(EXIT_SUCCESS);
}

//close current pipe_out since no longer use the out fd
//change pipe_in_fd to pre_pipe_in_fd since fd need to use next command input.
//also in_pipe_fd/out_pipe_fd should be set to -1
//if pre_in_pipe_fd is exist close
void	parent_process_fd_reset(t_context *ctx)
{
	if (ctx->pre_in_pipe_fd != -1 && ctx->pre_in_pipe_fd != STDIN_FILENO)
		close(ctx->pre_in_pipe_fd);
	if (ctx->out_pipe_fd != -1 && ctx->out_pipe_fd != STDOUT_FILENO)
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

int	exec_or_node(t_node *node, char **envp, t_context *ctx)
{
	ctx->is_exec_in_child_ps = false;
	if (!exec_handler(node->left, envp, ctx))
		return (EXIT_SUCCESS);
	else
		return (exec_handler(node->right, envp, ctx));
}

//int	exec_parenthi(t_node *node, char **envp,  t_context *ctx)
//{
//	pid_t pid;
//	int	c_status;
	
//	pid = fork();
//	if (pid == 0)
//		exec_handler(node->left, envp, ctx);
//	else
//	{
//		return (waitpid(pid, &c_status, 0));
//	}
//}
