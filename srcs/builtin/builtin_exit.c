/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:19:21 by ssoeno            #+#    #+#             */
/*   Updated: 2024/12/31 18:04:06 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtin.h"

int	builtin_exit(int argc, char *argv[], t_map *envmap, t_context *ctx)
{
	(void)envmap;
	(void)ctx;
	if (argc != 1)
	{
		printf("%s: too many arguments\n", argv[0]);
		return (EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
