/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 21:26:34 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/17 21:27:26 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void     exec_pipeline(struct cmd *cmdhead)
{
    struct cmd *cmd;
    int fds1[2] = {-1, -1};
    int fds2[2] = {-1, -1};

    for (cmd = cmdhead; cmd && !REDIRECT_P(cmd); cmd = cmd->next) {
        fds1[0] = fds2[0];
        fds1[1] = fds2[1];
        if (! TAIL_P(cmd)) {
            if (pipe(fds2) < 0) {
                perror("pipe");
                exit(3);
            }
        }
        if (lookup_builtin(cmd->argv[0]) != NULL) {
            cmd->pid = PID_BUILTIN;
        }
        else {
            cmd->pid = fork();
            if (cmd->pid < 0) {
                perror("fork");
                exit(3);
            }
            if (cmd->pid > 0) { /* parent */
                if (fds1[0] != -1) close(fds1[0]);
                if (fds1[1] != -1) close(fds1[1]);
                continue;
            }
        }
        if (! HEAD_P(cmd)) {
            close(0); dup2(fds1[0], 0); close(fds1[0]);
            close(fds1[1]);
        }
        if (! TAIL_P(cmd)) {
            close(fds2[0]);
            close(1); dup2(fds2[1], 1); close(fds2[1]);
        }
        if ((cmd->next != NULL) && REDIRECT_P(cmd->next)) {
            redirect_stdout(cmd->next->argv[0]);
        }
        if (!BUILTIN_P(cmd)) {
            execvp(cmd->argv[0], cmd->argv);
            fprintf(stderr, "%s: command not found: %s\n",
                    program_name, cmd->argv[0]);
            exit(1);
        }
    }
}

