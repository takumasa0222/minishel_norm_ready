/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:23:01 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 07:38:47 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"
#include "../../includes/expand_asterisk.h"

static size_t	get_split_count(char const *str, char dlm);

bool	is_aster_only(char *line)
{
	size_t	i;
	bool	is_aster_only_flg;

	i = 0;
	is_aster_only_flg = false;
	if (!line)
		throw_unexpected_error("is_aster_only", "line is NULL");
	while (line[i])
	{
		if (line[i] == ASTERISK)
		{
			is_aster_only_flg = true;
			i++;
		}
		else
		{
			is_aster_only_flg = false;
			break ;
		}
	}
	return (is_aster_only_flg);
}

t_cmp_str	*create_cmp_str(char *s, size_t st, size_t len, t_cmp_type typ)
{
	t_cmp_str	*ret;

	ret = xmalloc(sizeof(t_cmp_str));
	ret->compared_str = x_substr(s, (unsigned int)st, len);
	ret->compare_type = typ;
	return (ret);
}

bool	is_first_aster(char *str, size_t i)
{
	size_t	j;

	if (i < 1 || !str)
		throw_unexpected_error("is_first_aster", NULL);
	j = 0;
	while (j < i && str[j])
	{
		if (str[j] == ASTERISK)
			return (false);
		j++;
	}
	return (true);
}

size_t	skip_aster_sequence(char *line, size_t current)
{
	if (!line)
		throw_unexpected_error("is_first_aster", NULL);
	while (line[current] == ASTERISK)
		current++;
	return (current);
}

size_t	find_next_aster(char *line, size_t i)
{
	while (line[i])
	{
		if (line[i] == ASTERISK)
			return (i);
		i++;
	}
	return (0);
}

t_cmp_str	**init_cmp_str_arry(char *line)
{
	t_cmp_str	**ret;
	size_t		split_cnt;

	split_cnt = get_split_count(line, ASTERISK);
	ret = xmalloc(sizeof(t_cmp_str *) * (split_cnt + 1));
	return (ret);
}

static size_t	get_split_count(char const *str, char dlm)
{
	size_t	i;
	size_t	sp_cnt;

	i = 0;
	if (!str || !str[i])
		return (0);
	if (ft_strlen(str) == 1)
		return (1);
	sp_cnt = 0;
	while (str[i])
	{
		while (str[i] == dlm && str[i])
			i++;
		if (!str[i])
			break ;
		while (str[i] != dlm && str[i])
			i++;
		sp_cnt++;
	}
	return (sp_cnt);
}

t_cmp_str	**create_cmp_str_arry(char *line)
{
	t_cmp_str	**ret;
	size_t		i;

	if (!line)
		throw_unexpected_error("is_aster_only", "line is NULL");
	i = 0;
	ret = init_cmp_str_arry(line);
	if (is_aster_only(line))
		ret[i++] = create_cmp_str(line, 0, ft_strlen(line), ALL);
	else
		analyze_aster_loop(&ret, line, &i);
	ret[i] = NULL;
	return (ret);
}

void	analyze_aster_loop(t_cmp_str ***ret, char *line, size_t *x)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == ASTERISK)
		{
			if (i && is_first_aster(line, i))
				(*ret)[j++] = create_cmp_str(line, 0, i, PREFIX);
			j = analyze_sb_loop(ret, line, i, j);
			break ;
		}
		else
			i++;
	}
	*x = j;
}

size_t	analyze_sb_loop(t_cmp_str ***ret, char *line, size_t i, size_t j)
{
	size_t	nxt_ast;

	while (line[i])
	{
		i = skip_aster_sequence(line, i);
		nxt_ast = find_next_aster(line, i);
		if (!nxt_ast && i != ft_strlen(line))
		{
			(*ret)[j++] = create_cmp_str(line, i, ft_strlen(line), SUFFIX);
			break ;
		}
		else if (!nxt_ast)
			break ;
		else
			(*ret)[j++] = create_cmp_str(line, i, nxt_ast - i, CONTAIN);
		i = nxt_ast;
	}
	return (j);
}

//below code for expand asterisk. freeze for developping.
// void	expand_asterisk_handler(t_node *node)
// {
// 	int		i;
// 	char	**tmp;
// 	int		size;

// 	if (!node || !node->cmds)
// 		throw_unexpected_error("expand_variable_handler", NULL);
// 	i = 0;
// 	while (node->cmds[i])
// 	{
// 		size = 0;
// 		if (ft_strchr(node->cmds[i], ASTERISK))
// 		{
// 			tmp = NULL;
// 			// ここでリストの長さが変わる可能性があるので、詰めなおす必要がある
// 			tmp = expand_asterisk(node->cmds[i], &size);
// 			if (size > 1)
// 				recreate_command_list();
// 			free(node->cmds[i]);
// 			node->cmds[i] = NULL;
// 			node->cmds[i] = tmp;
// 		}
// 		i++;
// 	}
// }

char	**expand_asterisk(char *line)
{
	t_cmp_str	**cmp_arry;
	t_map		*file_map;
	char		**ret;

	ret = NULL;
	file_map = map_new();
	get_all_files_in_dir(file_map);
	cmp_arry = create_cmp_str_arry(line);
	filter_map(file_map, cmp_arry);
	ret = extract_file_name(file_map);
	return (ret);
}

void	get_all_files_in_dir(t_map *file_map)
{
	DIR				*dir;
	struct dirent	*dir_ent;

	dir = opendir(".");
	if (!dir)
		throw_system_error("expand_asterisk", "opendir failed");
	while (1)
	{
		dir_ent = readdir(dir);
		if (!dir_ent)
			break ;
		map_add_item(file_map, dir_ent->d_name, dir_ent->d_name);
	}
}

void	filter_map(t_map *file_map, t_cmp_str **cmp_arry)
{
	t_item	*file;
	t_item	*next_file;

	if (!cmp_arry || !file_map)
		throw_unexpected_error("filter_map", NULL);
	file = file_map->item_head.next;
	while (file)
	{
		next_file = file->next;
		if (!match_aster_regex(file->name, cmp_arry))
		{
			map_remove(file_map, file->name);
			file = NULL;
		}
		file = next_file;
	}
}

bool	match_aster_regex(char *f_name, t_cmp_str **cmp_arry)
{
	size_t	i;
	size_t	j;
	bool	is_match;

	is_match = true;
	if (!cmp_arry || !f_name)
		throw_unexpected_error("match_aster_regex", NULL);
	i = 0;
	j = 0;
	while (cmp_arry[i])
	{
		if (!j && cmp_arry[i]->compare_type == PREFIX)
			is_match = match_prefix(f_name, &j, cmp_arry[i]->compared_str);
		else if (cmp_arry[i]->compare_type == SUFFIX)
			is_match = match_suffix(f_name, &j, cmp_arry[i]->compared_str);
		else if (cmp_arry[i]->compare_type == CONTAIN)
			is_match = match_contain(f_name, &j, cmp_arry[i]->compared_str);
		// else if (cmp_arry[i]->compare_type == ALL)
		// 	is_match = match_all(f_name, cmp_arry[i]->compared_str);
		if (!is_match)
			return (is_match);
		i++;
	}
	return (is_match);
}

bool	match_prefix(char *f_name, size_t *i, char *prefix)
{
	size_t	j;

	j = 0;
	while (j < ft_strlen(prefix))
	{
		if (!f_name[j] || !(f_name[j] == prefix[j]))
			return (false);
		j++;
	}
	*i = j;
	return (true);
}

bool	match_suffix(char *f_name, size_t *i, char *suffix)
{
	size_t	j;
	size_t	f_name_len;
	size_t	suffix_len;

	j = 0;
	f_name_len = ft_strlen(f_name);
	suffix_len = ft_strlen(suffix);
	if (*i >= f_name_len || f_name_len < suffix_len + *i)
		return (false);
	while (j <= suffix_len && f_name[f_name_len - j] == suffix[suffix_len - j])
	{
		if (f_name_len < *i + j)
			return (false);
		if (!suffix[j])
			return (true);
		j++;
	}
	*i = j;
	return (false);
}

bool	match_contain(char *f_name, size_t *i, char *contain)
{
	size_t	k;
	size_t	file_name_len;
	size_t	contain_len;

	k = 0;
	file_name_len = ft_strlen(f_name);
	contain_len = ft_strlen(contain);
	if (*i >= file_name_len || file_name_len < contain_len + *i)
		return (false);
	while (f_name[*i])
	{
		k = 0;
		while (f_name[*i] == contain[k])
		{
			if (k == contain_len - 1)
				return ((*i)++, true);
			if (!f_name[*i])
				return (false);
			(*i)++;
			k++;
		}
		*i = *i - k + 1;
	}
	return (false);
}

char	**extract_file_name(t_map *file_map)
{
	size_t	item_cnt;
	t_item	*file;
	char	**ret;
	size_t	i;

	if (!file_map)
		return (NULL);
	item_cnt = map_size(file_map, false);
	if (!item_cnt)
		return (NULL);
	ret = xmalloc(sizeof(char *) * (item_cnt + 1));
	i = 0;
	file = file_map->item_head.next;
	while (file)
	{
		ret[i] = file->name;
		file = file->next;
		i++;
	}
	ret[i] = NULL;
	return (ret);
}
// bool	check_expandable_asterisk(char *str, size_t i)
// {
	
// }
