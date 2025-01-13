/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/13 21:39:59 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"
#include "../includes/redirect.h"
#include "../includes/execute.h"

void	redirect_in(char *filename);
void	redirect_out(char *filename);
void	redirect_append(char *filename);
void	redirect_here_doc(t_node *node);

void	redirect_here_doc(t_node *node)
{
	if (node->fd_num != -1)
		dup2(node->fd_num, STDIN_FILENO);
	close(node->fd_num);
}

void	redirect_in(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		d_throw_error("redirect_in", "Permission denied\n");
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		d_throw_error("redirect_in", "dup2_failed");
	}
	close(fd);
}

void	redirect_out(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		d_throw_error("redirect_out", "Permission denied\n");
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		d_throw_error("redirect_out", "dup2 failed");
	}
	close(fd);
}

void	redirect_append(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		d_throw_error("redirect_append", "Permission denied\n");
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		d_throw_error("redirect_append", "dup2 failed");
	}
	close(fd);
}
