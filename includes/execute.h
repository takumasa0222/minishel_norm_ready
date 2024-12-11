/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 18:09:09 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/12 01:06:59 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"
#include "./lexer.h"
#include "./parser.h"
#include <sys/types.h>
#include <sys/wait.h>

int			exec_handler(t_node *ast_node, char **envp, t_context *ctx);
void		exec_child_process(t_node *node, char **envp, t_context *ctx);
int			exec_pipe(t_node *node, char **envp, t_context *ctx);
int			exec_cmd(t_node *node, char **envp, t_context *ctx);
int			wait_child_status(t_context *ctx);
t_context	*init_ctx(void);
int			exec_cmd_handler(t_node *node, char **envp, t_context *ctx);
void		parent_process_fd_reset(t_context *ctx);
void		set_pipe_fd(int *in_fd, int *out_fd, int *pfd);
int			exec_or_node(t_node *node, char **envp, t_context *ctx);