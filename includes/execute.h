/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 18:09:09 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/15 01:57:34 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H
# include "./minishell.h"
# include "./lexer.h"
# include "./parser.h"
# include <sys/types.h>
# include <sys/wait.h>

# define EXIT	"exit"
# define CD "cd"
# define PWD "pwd"
# define UNSET "exit"
# define ENV "env"
# define ECHO "echo"
# define EXPORT "export"



int			exec_handler(t_node *ast_node, char **envp, t_context *ctx);
int			exec_pipe(t_node *node, char **envp, t_context *ctx);
int			exec_cmd(t_node *node, char **envp, t_context *ctx);
int			exec_cmd_handler(t_node *node, char **envp, t_context *ctx);

int			exec_or_node(t_node *node, char **envp, t_context *ctx);
int			exec_and_node(t_node *node, char **envp, t_context *ctx);
int			exec_round_brackets(t_node *node, char **envp, t_context *ctx);

void		setup_child_process_fd(t_context *ctx);
void		reset_parent_process_fd(t_context *ctx);
void		set_pipe_fd(int *in_fd, int *out_fd, int *pfd);

int			wait_children_status(t_context *ctx);
t_context	*init_ctx(void);
void		clear_pid_status(t_context *ctx);
bool		is_builtin(char *cmd);
#endif