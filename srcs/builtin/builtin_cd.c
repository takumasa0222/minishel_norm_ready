/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:55:17 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/12 22:50:15 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_cd(int argc, char *argv[], t_map *envmap)
{
	(void)envmap;
	if (argc > 2)
	{
		printf("%s: too many argument\n", argv[0]);
		return (EXIT_FAILURE);
	}
	if (argc == 1)
	{
		if (chdir(xgetenv(envmap, "HOME")) < 0)
		{
			perror("cd");
			return (EXIT_FAILURE);
		}
		return (EXIT_SUCCESS);
	}
	if (chdir(argv[1]) < 0)
	{
		perror(argv[1]);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
/*
if no arguments are given, change to the home directory
*/