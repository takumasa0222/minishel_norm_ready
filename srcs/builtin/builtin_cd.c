/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:55:17 by ssoeno            #+#    #+#             */
/*   Updated: 2024/12/30 21:57:21 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"
#include "../../includes/map.h"
#include "../../includes/environment.h"

/*
`cd -` simply swaps `PWD` and `OLDPWD` alternately.  
As a result, it repeatedly returns to the previous directory.  
`OLDPWD` always stores the current `PWD` and is used for subsequent `cd -`.
*/
// void	cd_with_dash(char *path, char *oldpwd)
// {
// 	if (oldpwd[0] == '\0')
// 	{
// 		printf("DEBUG cd_with_dash OLDPWD not set\n");
// 		path[0] = '\0';
// 		return ;
// 	}
// 	ft_strlcpy(path, oldpwd, PATH_MAX);
// 	return ;
// }
void	cd_with_no_argument(t_map *envmap, char *path)
{
	const char	*home;

	home = xgetenv(envmap, "HOME");
	if (home == NULL)
	{
		builtin_error("cd_with_no_argument", NULL, "HOME not set");
		path[0] = '\0';
		return ;
	}
	ft_strlcpy(path, home, PATH_MAX);
	return ;
}

int	builtin_cd(int argc, char *argv[], t_map *envmap)
{
	char		*oldpwd_before_chdir;
	char		*pwd_before_chdir;
	char		path[PATH_MAX];
	char		*pwd_after_chdir;

	oldpwd_before_chdir = xgetenv(envmap, "OLDPWD");
	pwd_before_chdir = xgetenv(envmap, "PWD");
	map_set(envmap, "OLDPWD", pwd_before_chdir);
	if (argc > 2)
	{
		printf("%s: too many argument\n", argv[0]);
		return (EXIT_FAILURE);
	}
	else if (argc == 1)
	{
		cd_with_no_argument(envmap, path);
	}
	else if (argc == 2 && argv[1][0] == '-')
	{
		builtin_error("cd", argv[1], "option not supported");
		return (EXIT_FAILURE);
	}
	else
	{
		ft_strlcpy(path, argv[1], PATH_MAX);
	}
	if (path[0] == '\0')
	{
		printf("cd: no such file or directory: %s\n", argv[1]);
		return (EXIT_FAILURE);
	}
	if (chdir(path) < 0)
	{
		builtin_error("cd", NULL, "chdir");
		return (EXIT_FAILURE);
	}
	pwd_after_chdir = resolve_pwd(pwd_before_chdir, path);
	map_set(envmap, "PWD", pwd_after_chdir);
	free(pwd_after_chdir);
	return (EXIT_SUCCESS);
}
/*
if no arguments are given, change to the home directory
"cd --" is out of subject!!! 
chdir only changes the current directory and 
does not update the logical path (PWD), 
so we need to update the PWD manually. 
*/
