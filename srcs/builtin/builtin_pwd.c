/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:56:59 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/12 22:57:14 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_pwd(int argc, char *argv[], t_map *envmap)
{
	char	buf[PATH_MAX];
	size_t	i;

	(void)envmap;
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == "-")
		{
			printf("%s: option unsupported\n", argv[i]);
			return (1);
		}
		i++;
	}
	if (!getcwd(buf, PATH_MAX))
	{
		printf("%s: cannot get working directory\n", argv[0]);
		return (1);
	}
	printf("%s\n", buf);
	return (0);
}
/*
if multiple arguments are given, ignore them
and print the current working directory
if argument contains "-", print error message
*/
