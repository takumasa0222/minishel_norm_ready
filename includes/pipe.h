/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 16:32:59 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/23 22:51:22 by ssoeno           ###   ########.fr       */
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

int		invoke_commands(struct cmds *cmdhead);
void	exec_pipeline(struct cmds *cmdhead, int cp_fd[2]);
int		wait_pipeline(struct cmds *cmdhead);
struct cmds*	pipeline_tail(struct cmds *cmdhead);
void    handle_parent_process(int *pfd_pre, int *pfd);
void   handle_child_process(struct cmds *cur_cmd,
    int *pfd_pre, int *pfd, int cp_fd[2]);

#endif