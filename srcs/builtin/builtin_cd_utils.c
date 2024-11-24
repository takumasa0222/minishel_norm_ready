/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:48:53 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/24 14:22:56 by ssoeno           ###   ########.fr       */
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
	while (*path)
	{
		if (*path == '/')
			last_slash_ptr = path;
		path++;
	}
	if (last_slash_ptr != NULL)
	{
		*last_slash_ptr = '\0';
		if (*path == '\0' && last_slash_ptr == start)
			ft_strlcpy(path, "/", 2);
	}
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

char	*resolve_pwd(char *pwd_before_chdir, char *path)
{
	char	pwd_after_chdir[PATH_MAX];
	char	*dup;

	if (pwd_before_chdir == NULL)
		return (NULL);
	if (*path == '/')
		ft_strlcpy(pwd_after_chdir, "/", PATH_MAX);
	else
		ft_strlcpy(pwd_after_chdir, pwd_before_chdir, PATH_MAX);
	while (*path)
	{
		if (*path == '/')
			path++;
		else if (consume_path(&path, path, "."))
			;
		else if (consume_path(&path, path, ".."))
			delete_last_elem(pwd_after_chdir);
		else
			append_path_elem(pwd_after_chdir, &path, path);
	}
	dup = ft_strdup(pwd_after_chdir);
	if (dup == NULL)
		d_throw_error("resolve_pwd", "Memory allocation error");
	return (dup);
}
/*
resolve_pwd is used to generate an absolute path
and set it to the PWD environment variable.
if the path starts with "/" (absolute path), 
	set pwd_after_chdir to "/"(root directory)
if the path starts with "." (relative path), \
	copy pwd_before_chdir to pwd_after_chdir
In the loop, 
'.' means the current directory so no need to change the path
if the path starts with ".." (relative path), 
copy pwd_before_chdir to pwd_after_chdir
*/