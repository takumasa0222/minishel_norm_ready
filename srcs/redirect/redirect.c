/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/08 00:56:58 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"
#include <unistd.h>
#include "../includes/redirect.h"

void	set_redirect_fds(t_node *node, t_context *ctx)
{
	// int	last_input_fd;
	// int	last_output_fd;

	// last_input_fd = -1;
	// last_output_fd = -1;
	// if (node->left)
	// {
	// 	open_files();
	// 	last_input_fd = retrieve_last_input_fd(node->left->redirects, last_input_fd);
	// 	last_output_fd = retrieve_last_output_fd(node->left->redirects, last_output_fd);
	// }
	// if (node->right)
	// {
	// 	open_files();
	// 	last_input_fd = retrieve_last_input_fd(node->right->redirects);
	// 	last_output_fd = retrieve_last_output_fd(node->right->redirects);
	// }

	// this is temporary implementation
	// if (!ctx->is_exec_in_child_ps)
	// 	store_std_in_out();
	if (ctx && node->left && node->left->fd_num != -1)
		dup2(node->left->fd_num, STDIN_FILENO);
	if (node->right && node->right->fd_num != -1)
		dup2(node->right->fd_num, STDIN_FILENO);
}
