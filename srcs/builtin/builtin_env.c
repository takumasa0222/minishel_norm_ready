/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:49:31 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/31 19:03:32 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtin.h"
#include "../../includes/utils.h"

int	builtin_env(int argc, char *argv[], t_context *ctx)
{
	t_item	*cur;

	(void) argv;
	if (argc > 1)
	{
		builtin_error("env", NULL, "Arguments are not supproted");
		ctx->last_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	cur = ctx->env->item_head.next;
	while (cur)
	{
		if (cur->value)
		{
			ft_putstr_fd(cur->name, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(cur->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		cur = cur->next;
	}
	ft_putendl_fd("_=/usr/bin/env", STDOUT_FILENO);
	ctx->last_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
/*
shell commands are executed with _ set to the path of the command
so that the command can know its own path
echo $_ prints the path of the command
*/
