/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/02/01 02:05:27 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"
#include "../includes/redirect.h"
#include "../includes/execute.h"

int	apply_redirects(t_node *node, t_context *ctx);

int	set_redirect_fds(t_node *node, t_context *ctx)
{
	if (!node)
		return (ERROR);
	if (!node->redirects)
		return (ERROR);
	return (apply_redirects(node, ctx));
}

int	apply_redirects(t_node *node, t_context *ctx)
{
	int		i;
	char	*filename;

	i = 0;
	while (node->redirects[i])
	{
		filename = node->redirects[i + 1];
		if (!filename)
			throw_unexpected_error("apply_redirect_fds", "filename is NULL");
		if (ft_strcmp(node->redirects[i], ">") == 0)
			redirect_out(filename, ctx);
		else if (ft_strcmp(node->redirects[i], ">>") == 0)
			redirect_append(filename, ctx);
		else if (ft_strcmp(node->redirects[i], "<") == 0)
			redirect_in(filename, ctx);
		else if (ft_strcmp(node->redirects[i], "<<") == 0)
			redirect_here_doc(node, ctx);
		else
			redirect_ambiguous_error(ctx);
		if (ctx->last_status != EXIT_SUCCESS)
			break ;
		i += 2;
	}
	return (ctx->last_status);
}

int	redirect_ambiguous_error(t_context *ctx)
{
	perror_prefix();
	ft_putendl_fd(" *: ambiguous redirect", STDERR_FILENO);
	ctx->last_status = EXIT_FAILURE;
	if (ctx->is_exec_in_child_ps)
		exit(ctx->last_status);
	return (EXIT_FAILURE);
}
