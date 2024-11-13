/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:19:21 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/13 21:49:19 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_exit(int argc, char *argv[], t_map *envmap)
{
	(void)envmap;
	if (argc != 1)
	{
		printf("%s: too many arguments\n", argv[0]);
		return (EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
