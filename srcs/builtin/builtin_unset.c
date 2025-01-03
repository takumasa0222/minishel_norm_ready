/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:50:01 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/01 15:21:16 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"

int	builtin_unset(int argc, char *argv[], t_map *envmap, t_context *ctx)
{
	size_t	i;

	(void)argc;
	i = 1;
	if (envmap == NULL)
	{
		builtin_error("builtin_unset", NULL, "map is not initialized");
		ctx->last_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	while (argv[i])
	{
		if (map_unset(envmap, argv[i]) < 0)
		{
			builtin_error("unset", argv[i], "not a valid identifier");
			ctx->last_status = 1;
		}
		else
			ctx->last_status = 0;
		i++;
	}
	return (ctx->last_status);
}
