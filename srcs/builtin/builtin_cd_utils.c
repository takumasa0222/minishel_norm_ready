/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:48:53 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/14 01:07:26 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"

bool	consume_path(char **rest, char *path, char *elem)
{
	size_t	elem_len;

	elem_len = ft_strlen(elem);
	if (ft_strncmp(path, elem, elem_len) == 0)//CHECK strcmp should be used?
	{
		if (path[elem_len] == '\0' || path[elem_len] == '/')
		{
			*rest = (char *)(path + elem_len);
			return (true);
		}
	}
	return (false);
}
/*
example:
path = "../projects"
result = consume_path(&rest, path, "..")
result = true
ret == "projects"
*/

void	delete_last_elem(char *path)
{
	char	*cur;
	char	*last_slash_ptr;

	if (*path == '\0' || (*path == '/' && ft_strlen(path) == 1))
		return ;
	cur = path;
	last_slash_ptr = NULL;
	while (*cur)
	{
		if (*cur == '/')
			last_slash_ptr = cur;
		cur++;
	}
	if (last_slash_ptr)
		*last_slash_ptr = '\0';
	if (*path == '\0')
		ft_strlcpy(path, "/", 2);
}
/*
if path is "/"(root directory), do nothing.
if path is "/a/b/c", delete "c" and return "/a/b"
if path is null character after deleting the last element, return "/"
*/

void	append_path_elem(char *dst, char **rest, char *src)
{
	size_t	elem_len;

	elem_len = 0;
	while (src[elem_len] && src[elem_len] != '/')
		elem_len++;
	if (dst[ft_strlen(dst) - 1] != '/')
		ft_strlcat(dst, "/", PATH_MAX);
	if (elem_len > 0)
		ft_strlcat(dst, src, ft_strlen(dst) + elem_len + 1);
	*rest = (char *)(src + elem_len);
}
// not sure strlcat is the best choice here
/*
Example1: when dst does not end with '/'
dst[PATH_MAX] = "/home/user";
src = "projects/next"
append_path_elem(dst, &rest, src);
dst == "/home/user/projects" pwd
rest == "next"

Example2: when dst ends with '/'
dst[PATH_MAX] = "/";
src = "home/user/projects"
while(*rest){
	append_path_elem(dst, &rest, src);
	src = rest;
}
dst == "/home/user/projects"
rest == ""
*/

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
	dup = ft_strdup(pwd_after_chdir);//system error xmalloc should be used
	if (dup == NULL)
		d_throw_error("resolve_pwd", "Memory allocation error");
	return (dup);
}
/*
resolve_pwd is used to generate an absolute path
and set it to the PWD environment variable.
if the path starts with "/" (absolute path), 
	initialize pwd_after_chdir with "/"(root)
if the path starts with "." (relative path), \
	copy pwd_before_chdir to pwd_after_chdir
In the loop, 
'.' means the current directory so no need to change the path
if the path starts with ".." (relative path), 
copy pwd_before_chdir to pwd_after_chdir

example:
pwd_before_chdir = "/home/user"
path = "../usr/bin"
pwd_after_chdir = "/home/usr/bin"

pwd_before_chdir = "/home/ssoeno"
path = "minishell"
pwd_after_chdir = "/home/user/minishell"

pwd_before_chdir = "/home/user/minishell/minishell_norm_ready"
path = "./minishell/../../pwd_before_chdirusr/bin"
pwd_after_chdir = "/usr/bin"

pwd_before_chdir = "/home/user"
path = "projects/../next";
step1 append_path_elem adds "projects" to dst
	dst == "/home/user/projects"
	rest == "../next"
step2 consume_path detect ".." -> go back to the parent dir
	*delete_last_elem
	dst = "/home/user"
	rest == "next"
step3 append_path_elem adds "next" to dst
	dst == "/home/user/next"
	rest == ""
*/
