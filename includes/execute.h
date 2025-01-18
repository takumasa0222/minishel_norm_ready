/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 18:09:09 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/18 18:16:04 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H
# include "./minishell.h"
# include "./lexer.h"
# include "./parser.h"
# include <sys/types.h>
# include <sys/wait.h>
# include "./map.h"

# define EXIT "exit"
# define CD "cd"
# define PWD "pwd"
# define UNSET "unset"
# define ENV "env"
# define ECHO "echo"
# define EXPORT "export"

int		exec_handler(t_node *ast_node, t_context *ctx);
int		exec_pipe(t_node *node, t_context *ctx);
int		exec_cmd(t_node *node, t_context *ctx);
int		exec_cmd_handler(t_node *node, t_context *ctx);

int		exec_or_node(t_node *node, t_context *ctx);
int		exec_and_node(t_node *node, t_context *ctx);
int		exec_round_brackets(t_node *node, t_context *ctx);

void	setup_child_process_fd(t_context *ctx);
void	reset_parent_process_fd(t_context *ctx);
void	set_pipe_fd(int *in_fd, int *out_fd, int *pfd);

int		wait_children_status(t_context *ctx);
void	clear_pid_status(t_context *ctx);
bool	is_builtin(char *cmd);

// exec_builtin.c
void	backup_std_fds(t_context *ctx);
void	restore_std_fds(t_context *ctx);
int		exec_builtin(char *cmd, char **argv, t_context *ctx);
int		run_builtin(t_node *node, t_context *ctx);

// exec_external.c
// char	*resolve_executable_path(t_node *node, t_map *envp);
int		run_external(t_node *node, t_context *ctx);

// exec_search_path_env.c
char	*search_path_env_or_exit(t_node *node, t_map *envp);

// exec_error.c
void	exit_command_not_found(char *cmd);
void	exit_permission_denied(char *cmd);
void	exit_file_not_found(char *cmd);
void	exit_is_directory(char *cmd);

#endif