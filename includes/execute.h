/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 18:09:09 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/08 18:39:19 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"
#include "./lexer.h"
#include "./parser.h"
#include <sys/types.h>
#include <sys/wait.h>

void		exec_handler(t_node *ast_node, char **envp, t_context *ctx);
void		exec_child_process(t_node *node, char **envp, t_context *ctx);
void		exec_pipeline(t_node *node, char **envp, t_context *ctx);
void		exec_cmd(t_node *node, char **envp, t_context *ctx);
t_context	*init_ctx(void);
void		exec_cmd_handler(t_node *node, char **envp, t_context *ctx);
void		parent_process_fd_reset(t_context *ctx);
void		set_pipe_fd(int *in_fd, int *out_fd, int *pfd);