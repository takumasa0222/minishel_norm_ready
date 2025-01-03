/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 21:14:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/04 02:43:46 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"

/*
$? -> 前に終了したステータスの中身を文字列として取り出す
$<変数名> -> 変数名に該当する変数を展開し、中身を文字列として取り出す
*展開 -> カレントディレクト下のファイル・フォルダ名に展開することが出来る。
シングルクォーテーションで囲まれている場合、展開は行わない
ダブルクォーテーションで囲まれている場合、$変数の展開は行う
*/

void	expand_handler(t_node *node, char **envp)
{
	if (!node || !node->cmds)
		d_throw_error("expand_handler", "node or cmds is null");
	expand_variable_handler(node, envp);
	//word_split_handler(node);
	//expand_asterisk_handler(node);
	//remove_quotes_handler(node);
}

void	expand_variable_handler(t_node *node, char **envp)
{
	int		i;
	char	*tmp;

	if (!node || !node->cmds)
		d_throw_error("expand_variable_handler", "node or cmds is null");
	i = 0;
	while (node->cmds[i])
	{
		if (ft_strchr(node->cmds[i], DOLLAR))
		{
			tmp = NULL;
			tmp = expand_variable(node->cmds[i], envp);
			free(node->cmds[i]);
			node->cmds[i] = NULL;
			node->cmds[i] = tmp;
		}
		i++;
	}
}

char	*expand_variable(char *str, char **envp)
{
	size_t	i;
	char	*ret;

	i = 0;
	ret = NULL;
	while (str[i])
	{
		if (is_s_quote(str[i]))
			i = retrieve_val_in_sq(&ret, str, i);
		else if (is_dollar_symbol(str[i]))
			i = retrieve_var(&ret, str, i, envp);
		else if (is_d_quote(str[i]))
			i = retrieve_val_in_dq(&ret, str, i, envp);
		else
			i = retrieve_normal_val(&ret, str, i);
		i++;
	}
	return (ret);
}

size_t	retrieve_val_in_sq(char **ret, char *str, size_t i)
{
	size_t	next_sq_pos;

	if (!is_s_quote(str[i]) || !ret || !str)
		d_throw_error("retrieve_val_in_sq", "arg is invalid");
	next_sq_pos = skip_s_quote(str, i);
	str_concat_helper(ret, str, i, next_sq_pos - i + 1);
	if (!is_s_quote(str[next_sq_pos]))
		d_throw_error("retrieve_val_in_sq", "unexpected error");
	return (next_sq_pos);
}

size_t	skip_s_quote(char *str, size_t i)
{
	if (!str)
		d_throw_error("skip_s_quote", "str is null");
	i = i + 1;
	while (str[i] && !is_s_quote(str[i]))
		i++;
	return (i);
}

bool	is_dollar_symbol(char c)
{
	return (c == DOLLAR);
}

size_t	retrieve_val_in_dq(char **ret, char *str, size_t i, char **envp)
{
	size_t	j;

	if (!is_d_quote(str[i]) || !ret || !str)
		d_throw_error("retrieve_val_in_dq", "arg is invalid");
	j = i + 1;
	while (str[j] && !is_d_quote(str[j]))
	{
		if (is_dollar_symbol(str[j]))
		{
			str_concat_helper(ret, str, i, j - i);
			j = retrieve_var(ret, str, j, envp);
			i = j + 1;
		}
		j++;
	}
	str_concat_helper(ret, str, i, j - i + 1);
	if (!is_d_quote(str[j]))
		d_throw_error("retrieve_val_in_dq", "unexpected error");
	return (j);
}

char	*str_concat_helper(char **ret, char *str, size_t i, size_t len)
{
	char	*tmp;
	char	*ret_val;

	tmp = ft_substr(str, i, len);
	if (!tmp)
		d_throw_error("str_concat_helper", "substr failed");
	ret_val = ft_strjoin(*ret, tmp);
	if (!ret_val)
		d_throw_error("str_concat_helper", "strjoin failed");
	free(tmp);
	free(*ret);
	*ret = NULL;
	*ret = ret_val;
	return (ret_val);
}

size_t	retrieve_var(char **ret, char *str, size_t i, ctx)
{
	size_t	var_name_len;
	char	*tmp;
	char	*map_get_val;
	t_map	*map;

	if (!str)
		d_throw_error("retrieve_var", "arg is invalid");
	if (!str[i + 1])
		return (str_concat_helper(ret, str, i, 1), i);
	var_name_len = retrieve_var_name_len(str, i + 1);
	if (!var_name_len)
		return (str_concat_helper(ret, str, i, 1), i);
	tmp = ft_substr(str, i + 1, var_name_len);
	if (!tmp)
		d_throw_error("retrieve_var", "substr failed");
	map = init_envmap(envp);
	map_get_val = map_get(map, tmp);
	if (map_get_val)
		str_concat_helper(ret, map_get_val, 0, ft_strlen(map_get_val));
	else
		str_concat_helper(ret, "", 0, 0);
	free(tmp);
	return (var_name_len + i);
}

size_t	retrieve_var_name_len(char *str, size_t i)
{
	size_t	len;

	len = 0;
	if (!str)
		d_throw_error("retrieve_var_name_end", "arg is invalid");
	if (is_d_quote(str[i]) || is_s_quote(str[i]))
		return (0);
	if (!ft_isalpha(str[i]) && str[i] != UNDER_SCORE)
		return (1);
	while (ft_isalnum(str[i + len]) || str[i + len] == UNDER_SCORE)
		len++;
	return (len);
}

size_t	retrieve_normal_val(char **ret, char *str, size_t i)
{
	size_t	j;

	if (!str)
		d_throw_error("retrieve_normal_val", "arg is invalid");
	j = 0;
	while (str[i + j] && !is_d_quote(str[i + j]) && \
	!is_s_quote(str[i + j]) && !is_dollar_symbol(str[i + j]))
	{
		j++;
	}
	str_concat_helper(ret, str, i, j);
	return (i + j - 1);
}

// t_map	*init_envmap(char **envp)
// {
// 	int		i;
// 	t_map	*map;

// 	if (!envp)
// 		d_throw_error("init_envap", "invalid envp");
// 	map = map_new();
// 	i = 0;
// 	while (envp[i])
// 	{
// 		map_put(map, envp[i], false);
// 		i++;
// 	}
// 	return (map);
// }

// #include <sys/types.h>
// #include <dirent.h>
// void	expand_asterisk_handler(t_node *node)
// {
// 	int		i;
// 	char	**tmp;
// 	int		size;

// 	if (!node || !node->cmds)
// 		d_throw_error("expand_variable_handler", "node or cmds is null");
// 	i = 0;
// 	while (node->cmds[i])
// 	{
// 		size = 0;
// 		if (ft_strchr(node->cmds[i], ASTERISK))
// 		{
// 			tmp = NULL;
// 			// ここでリストの長さが変わる可能性があるので、詰めなおす必要がある
// 			tmp = expand_asterisk(node->cmds[i], &size);
// 			if (size > 1)++
// 				recreate_command_list();
// 			free(node->cmds[i]);
// 			node->cmds[i] = NULL;
// 			node->cmds[i] = tmp;
// 		}
// 		i++;
// 	}
// }

// char	**expand_asterisk(char *str, int *size)
// {
// 	size_t	i;
// 	bool	is_first_asterisk;	

// 	i = 0;
// 	is_first_asterisk = false;
// 	while (str[i])
// 	{
// 		i = find_next_asterisk(str);
// 		if (str[i] != ASTERISK && is_first_asterisk)
// 			filtered_files = find_end_match_file(str, i);
// 		else if (str[i] == ASTERISK && is_first_asterisk)
// 			filtered_files = find_contain_file(str, i);
// 		else if (str[i] == ASTERISK && !is_first_asterisk)
// 		{
// 			find_start_match_file(str, i);
// 			filtered_files = is_first_asterisk = true;
// 		}
// 	}

// }

// get_all_files_in_dir()
// {
// 	DIR		*dir;
// 	dirent	*dir_ent;
	
// 	dir = opendir(".");
// 	if (!dir)
// 		d_throw_error("expand_asterisk", "opendir failed");
// 	while ()
// 	{
// 		dir_ent = readdir(dir);
// 		dir_ent->dir_name
// 	}
// }

// bool	check_expandable_asterisk(char *str, size_t i)
// {
	
// }

// size_t	find_next_asterisk(char *str)
// {
// 	size_t	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (is_s_quote(input[i]) || is_d_quote(input[i]))
// 			i = move_to_next_quotation(input, i);
// 		if (str[i] == ASTERISK)
// 			return (i);
// 		i++;
// 	}
// 	return (i);
// }
