/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 16:32:59 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/25 21:24:50 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_H
# define PIPE_H

struct cmds {
	int			argc;
	char		**cmd;
	pid_t		pid;
	int			exit_status;
	struct cmds	*next;
};

// struct s_node {
// 	t_node_kind	kind;
// 	t_node		*left;
// 	t_node		*right;
// 	char		**cmds;
// 	char		*op_val;
// 	int			fd_num;
// 	char		**redirects;
// };

/**
* @brief Executes a sequence of commands in a pipeline
* @param cmdhead Pointer to the first command in the pipeline
* @return Exit status of the last command
*/
int				invoke_commands(struct cmds *cmdhead);
/**
* @brief Executes commands in a pipeline with specified file descriptors
* @param cmdhead Pointer to the first command
* @param cp_fd Array of file descriptors for the pipeline
*/
void			exec_pipeline(struct cmds *cmdhead, int cp_fd[2]);
/**
* @brief Waits for all processes in the pipeline to complete
* @param cmdhead Pointer to the first command
* @return Exit status of the last command
*/
int				wait_pipeline(struct cmds *cmdhead);
struct cmds		*pipeline_tail(struct cmds *cmdhead);
void			handle_parent_process(int *pfd_pre, int *pfd);
void			handle_child_process(struct cmds *cur_cmd,
		int *pfd_pre, int *pfd, int cp_fd[2]);

#endif