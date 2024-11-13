/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:55:17 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/13 23:20:31 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"

int	builtin_cd(int argc, char *argv[], t_map *envmap)
{
	const char	*home;

	if (argc == 1)
	{
		home = xgetenv(envmap, "HOME");
		if (home == NULL)
		{
			builtin_error("cd", argv[0], "HOME not set");
			return (EXIT_FAILURE);
		}
	}
	if (argc > 2)
	{
		printf("%s: too many argument\n", argv[0]);
		return (EXIT_FAILURE);
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
