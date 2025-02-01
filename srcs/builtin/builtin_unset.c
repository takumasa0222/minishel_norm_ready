/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:50:01 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/31 19:00:48 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"

int	builtin_unset(int argc, char *argv[], t_context *ctx)
{
	size_t	i;

	(void)argc;
	i = 1;
	if (ctx->env == NULL)
	{
		builtin_error("builtin_unset", NULL, "map is not initialized");
		ctx->last_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	while (argv[i])
	{
		if (!is_identifier(argv[i]))
		{
			// builtin_error("unset", argv[i], "not a valid identifier");
			ctx->last_status = 0;
			i++;
			continue ;
		}
		map_unset(ctx->env, argv[i]);
		i++;
	}
	return (ctx->last_status);
}
/*
if the name is not exist in the map, do nothing and return 0
(we ignore the return value of map_unset)
unsupported opsions are ignored (return "not a valid identifier")
*/