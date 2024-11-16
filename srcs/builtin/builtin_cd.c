/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:55:17 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/16 20:59:02 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"

bool	consume_path(char **rest, char *path, char *elem)
{
	size_t	elem_len;

	elem_len = ft_strlen(elem);
	if (ft_strncmp(path, elem, elem_len) == 0)
	{
		if (path[elem_len] == '\0' || path[elem_len] == '/')
		{
			*rest = path + elem_len;
			return (true);
		}
	}
	return (false);
}

void	delete_last_elem(char *path)
{
	char	*start;
	char	*last_slash_ptr;
	
	if (*path == '\0' || (*path == '/' && ft_strlen(path) == 1))
		return ;
	start = path;
	last_slash_ptr = NULL;
	// printf("DEBUG delete_last_elem path before loop: %s\n", path);
	while (*path)
	{
		if (*path == '/')
			last_slash_ptr = path;
		path++;
	}
	// printf("DEBUG delete_last_elem path: %s\n", path);
	// printf("DEBUG delete_last_elem last_slash_ptr: %s\n", last_slash_ptr);
	if (last_slash_ptr != NULL)
	{
		*last_slash_ptr = '\0';
		if (*path == '\0' && last_slash_ptr == start)
			ft_strlcpy(path, "/", 2);
	}
	printf("DEBUG delete_last_elem path: %s\n", path);
}
/*
if path is "/"(root directory), do nothing.
if path is "/a/b/c", delete "c" and return "/a/b"
if path is null character after deleting the last element, return "/"
*/ 

void	append_path_elem(char *dst, char **rest, char *src)
{
	size_t	elem_len;
	char	*temp_elem;

	elem_len = 0;
	temp_elem = ft_calloc(PATH_MAX, sizeof(char));
	while (src[elem_len] && src[elem_len] != '/')
		elem_len++;
	if (dst[ft_strlen(dst) - 1] != '/')
		ft_strlcat(dst, "/", PATH_MAX);
	if (elem_len > 0)
	{
		ft_strlcat(temp_elem, src, elem_len + 1);
		ft_strlcat(dst, temp_elem, PATH_MAX);
	}
	*rest = src + elem_len;
	free(temp_elem);
}
// not sure strlcat is the best choice here

char	*resolve_pwd(char *oldpwd, char *path)
{
	char 	newpwd[PATH_MAX];
	char	*dup;

	if (oldpwd == NULL)
		return (NULL);
	if (*path == '/')
		ft_strlcpy(newpwd, "/", PATH_MAX);
	else
		ft_strlcpy(newpwd, oldpwd, PATH_MAX);
	while (*path)
	{
		if (*path == '/')
			path++;
		else if (consume_path(&path, path, "."))
			;
		else if (consume_path(&path, path, ".."))
			delete_last_elem(newpwd);
		else
			append_path_elem(newpwd, &path, path);
	}
	dup = ft_strdup(newpwd);
	// printf("DEBUG dup: %s\n", dup);
	if (dup == NULL)
		d_throw_error("resolve_pwd", "Memory allocation error");
	return (dup);
}
/*
resolve_pwd is used to generate an absolute path
and set it to the PWD environment variable.
if the path starts with "/" (absolute path), set newpwd to "/"(root directory)
if the path starts with "." (relative path), copy oldpwd to newpwd
In the loop, 
'.' means the current directory so no need to change the path
if the path starts with ".." (relative path), copy oldpwd to newpwd
*/

// {"cd", 1, {"cd", NULL}, "Change to HOME directory when no arguments are provided"},
int	builtin_cd(int argc, char *argv[], t_map *envmap)
{
	const char	*home;
	char 		*oldpwd;
	char 		*previous_pwd;
	char 		path[PATH_MAX];
	char		*newpwd;

	previous_pwd = xgetenv(envmap, "OLDPWD");
	oldpwd = xgetenv(envmap, "PWD");
	map_set(envmap, "OLDPWD", oldpwd);
	if (argc > 2)
	{
		printf("%s: too many argument\n", argv[0]);
		return (EXIT_FAILURE);
	}
	if (argc == 1)
	{
		home = xgetenv(envmap, "HOME");
		if (home == NULL)
		{
			perror("HOME not set");
			return (EXIT_FAILURE);
		}
		ft_strlcpy(path, home, PATH_MAX);
	}
	else
	{
		if (argc == 2 && argv[1][0] == '-' && ft_strlen(argv[1]) == 1)
		{
			if (previous_pwd[0] == '\0')
			{
				printf("OLDPWD not set\n");
				return (EXIT_FAILURE);
			}
			ft_strlcpy(path, previous_pwd, PATH_MAX);
		}
		else
			ft_strlcpy(path, argv[1], PATH_MAX);
	}
	if (chdir(path) < 0)
	{
		perror("chdir");
		// builtin_error("cd", NULL, "chdir");
		return (EXIT_FAILURE);
	}
	printf("DEBUG path before resolve_pwd: %s\n", path);
	printf("DEBUG oldpwd before resolve_pwd: %s\n", oldpwd);
	newpwd = resolve_pwd(oldpwd, path);
	printf("DEBUG newpwd after resolve_pwd: %s\n", newpwd);
	printf("DEBUG oldpwd after resolve_pwd: %s\n", oldpwd);
	map_set(envmap, "PWD", newpwd);
	printf("DEBUG previous pwd after map_set(envmap, PWD, newpwd) : %s\n", previous_pwd);
	// map_set(envmap, "OLDPWD", previous_pwd);
	free(newpwd);
	return (EXIT_SUCCESS);
}
/*
if no arguments are given, change to the home directory
"cd --" is out of subject!!! 
chdir only changes the current directory and 
does not update the logical path (PWD), 
so we need to update the PWD manually. 
*/
