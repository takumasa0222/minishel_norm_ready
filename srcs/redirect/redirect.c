/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/02/01 09:55:44 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"
#include "../includes/redirect.h"
#include "../includes/execute.h"

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
	size_t	i;
	char	**rds;

	i = 0;
	rds = node->redirects;
	while (rds[i])
	{
		if (!rds[i + 1])
			throw_unexpected_error("apply_redirect_fds", "filename is NULL");
		if (ft_strcmp(rds[i], ">") == 0 && is_multi_file(rds, i))
			redirect_out(node->redirects[i + 1], ctx);
		else if (ft_strcmp(rds[i], ">>") == 0 && is_multi_file(rds, i))
			redirect_append(node->redirects[i + 1], ctx);
		else if (ft_strcmp(rds[i], "<") == 0 && is_multi_file(rds, i))
			redirect_in(node->redirects[i + 1], ctx);
		else if (ft_strcmp(rds[i], "<<") == 0 && is_multi_file(rds, i))
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

bool	is_multi_file(char **redirects, size_t i)
{
	if (!redirects || !redirects[i] || !redirects[i + 1])
		throw_unexpected_error("check_multi_file", "null error");
	if (!redirects[i + 2])
		return (true);
	if (ft_strcmp(redirects[i + 2], ">") == 0)
		return (true);
	else if (ft_strcmp(redirects[i + 2], ">>") == 0)
		return (true);
	else if (ft_strcmp(redirects[i + 2], "<") == 0)
		return (true);
	else if (ft_strcmp(redirects[i + 2], "<<") == 0)
		return (true);
	else
		return (false);
}
