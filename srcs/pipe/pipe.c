/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 18:31:50 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/23 18:33:05 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/pipe.h"

int	invoke_commands(struct cmds *cmdhead)
{
	int	exit_status;
	int	original_stdin;
	int	original_stdout;

	original_stdin = dup(0);
	original_stdout = dup(1);
	exec_pipeline(cmdhead);
	exit_status = wait_pipeline(cmdhead);
	close(0);
	dup2(original_stdin, 0);
	close(original_stdin);
	close(1);
	dup2(original_stdout, 1);
	close(original_stdout);
	return (exit_status);
}

void	exec_pipeline(struct cmds *cmdhead)
{
	struct cmds	*cur_cmd;
	int			pfd_pre[2];
	int			pfd[2];

	cur_cmd = cmdhead;
	ft_memset(pfd_pre, -1, sizeof(pfd_pre));
	ft_memset(pfd, -1, sizeof(pfd));
	while (cur_cmd)
	{
		pfd_pre[0] = pfd[0];
		pfd_pre[1] = pfd[1];
		if (cur_cmd->next != NULL && pipe(pfd) < 0)
			d_throw_error("exec_pipeline", "failed to create pipe");
		cur_cmd->pid = fork();
		if (cur_cmd->pid < 0)
			d_throw_error("exec_pipeline", "failed to fork");
		if (cur_cmd->pid > 0)
		{
			handle_parent_process(pfd_pre, pfd);
			cur_cmd = cur_cmd->next;
			continue ;
		}
		handle_child_process(cmdhead, cur_cmd, pfd_pre, pfd);
	}
}

void	handle_parent_process(int *pfd_pre, int *pfd)
{
	if (pfd_pre[0] != -1)
		close(pfd_pre[0]);
	if (pfd_pre[1] != -1)
		close(pfd_pre[1]);
	if (pfd[1] != -1)
		close(pfd[1]);
}

void	handle_child_process(struct cmds *cmdhead,
	struct cmds *cur_cmd, int *pfd_pre, int *pfd)
{
	if (cur_cmd != cmdhead)
	{
		close(STDIN_FILENO);
		dup2(pfd_pre[0], STDIN_FILENO);
		close(pfd_pre[0]);
		close(pfd_pre[1]);
	}
	if (cur_cmd->next != NULL)
	{
		close(pfd[0]);
		close(STDOUT_FILENO);
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
	}
	execvp(cur_cmd->cmd[0], cur_cmd->cmd);
	d_throw_error("exec_pipeline", "failed to execvp");
}

/*
if cmd is a builtin
	call the function and return exit status
if cmd is not a builtin
	wait for the child process to finish
	and return the exit status
*/
int	wait_pipeline(struct cmds *cmdhead)
{
	struct cmds	*cmd;

	cmd = cmdhead;
	while (cmd->next)
	{
		waitpid(cmd->pid, &cmd->exit_status, 0);
		cmd = cmd->next;
	}
	return (pipeline_tail(cmdhead)->exit_status);
}

struct cmds	*pipeline_tail(struct cmds *cmdhead)
{
	struct cmds	*cmd;

	cmd = cmdhead;
	while (cmd->next)
		cmd = cmd->next;
	return (cmd);
}
/*
libftのft_lstlastと同じ機能
lexer, parserで連結リストの操作関数を使っているなら代替可能
*/