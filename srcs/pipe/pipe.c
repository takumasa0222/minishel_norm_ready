/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 21:26:34 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/19 21:14:45 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <sys/wait.h>

void    exec_pipeline(char ***cmds)
{
    int i = 0; // index for cmds
    int in_fd = STDIN_FILENO; // pipe read end
    int pipe_fd[2]; // fd[0] is read end, fd[1] is write end of the pipe
    pid_t pid; // child process id
    int status; // exit status of the child process

    while (cmds[i] != NULL)
    {
        if (cmds[i + 1] != NULL) // if not the last command, create a pipe
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        pid = fork(); // create a child process
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) // child process
        {
            if (in_fd != STDIN_FILENO)
            {
                dup2(in_fd, STDIN_FILENO); // change the STDIN to in_fd
                close (in_fd);
            }
            if (cmds[i + 1] != NULL)
            {
                dup2(pipe_fd[1], STDOUT_FILENO); // change the STDOUT to pipe_fd[1]
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            execvp(cmds[i][0], cmds[i]); // execute the command
            perror("execvp"); // if execvp fails
            exit(EXIT_FAILURE);
        }

        // parent process
        /*
        if_fdは、現在のコマンドに渡す入力を保持している。
        一度使ったあとはin_fdは不要になるので閉じる。
        */
        if (in_fd != STDIN_FILENO) // if not the first command
            close(in_fd); // close the read end of the pipe
        if (cmds[i + 1] != NULL)
        {
            close(pipe_fd[1]); // parent process does not need the write end of the pipe
            in_fd = pipe_fd[0]; // update in_fd to the read end of the pipe
            // next command will redirect its STDIN to in_fd
        }
        /*
        親プロセスでは、次のコマンドの入力を作るために、pipe_fd[0]（read）を保持
        pipe_fd[1]（write）は使わないので閉じる。
        */
        i++;
    }

    // wait for all the child processes to finish
    while ((pid = wait(&status)) > 0)
    {
        if (WIFEXITED(status))
            printf("exited, status=%d\n", WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("killed by signal %d\n", WTERMSIG(status));
        else if (WIFSTOPPED(status))
            printf("stopped by signal %d\n", WSTOPSIG(status));
        else if (WIFCONTINUED(status))
            printf("continued\n");
    }
}

/*
処理のフロー
最初のコマンド:
- 標準入力（STDIN_FILENO）をそのまま使用。
- 出力を次のコマンドの入力用パイプ（pipe_fd[1]）にリダイレクト。
- 子プロセスで pipe_fd[0] を閉じる。

中間のコマンド:
- 入力を前のコマンドの出力（in_fd）にリダイレクト。
- 出力を次のコマンドの入力用パイプ（pipe_fd[1]）にリダイレクト。
- 親プロセスで使い終わった in_fd と pipe_fd[1] を閉じ、in_fd を更新。

最後のコマンド:
- 入力を前のコマンドの出力（in_fd）にリダイレクト。
- 出力は標準出力（STDOUT_FILENO）にリダイレクトされたまま。
- 子プロセスで使わない pipe_fd[0] を閉じる。
*/

int main()
{
    size_t i = 0;
    char *cmd1[] = {"echo", "hello\nworld", NULL};
    char *cmd2[] = {"wc", "-l", NULL};
    char *cmd3[] = {"cat", NULL};
    char **cmds[] = {cmd1, cmd2, cmd3, NULL};
    while (cmds[i] != NULL)
    {
        exec_pipeline(cmds[i]);
        i++;
    }
    return 0;
}
