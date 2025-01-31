/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:23:01 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 15:00:25 by tamatsuu         ###   ########.fr       */
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
	ret->cmp_str = x_substr(s, (unsigned int)st, len);
	ret->cmp_type = typ;
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
 void	expand_asterisk_handler(t_node *node)
 {
 	size_t	i;
 	char	**tmp;

 	if (!node || !node->cmds)
 		throw_unexpected_error("expand_variable_handler", NULL);
 	i = 0;
 	while (node->cmds[i])
 	{
 		if (ft_strchr(node->cmds[i], ASTERISK))
 		{
 			tmp = NULL;
 			tmp = expand_asterisk(node->cmds[i]);
 			if (tmp)
 				i = i + recreate_command_list(node, tmp, i);
 		}
 		i++;
 	}
}

size_t	recreate_command_list(t_node *node, char **file_arry, size_t i)
{
	size_t	ret;
	size_t	origin_arry_size;
	size_t	new_arry_size;
	char	**new_cmds;

	ret = 0;
	if (!node || !file_arry)
		return (ret);
	origin_arry_size = get_char_arry_size(node->cmds);
	new_arry_size = get_char_arry_size(file_arry);
	new_cmds = xmalloc(sizeof(char *) * (origin_arry_size + new_arry_size - 2));
	ret = pack_new_cmds(node->cmds, file_arry, new_cmds, i);
	free_wordlist(&node->cmds);
	free_wordlist(&file_arry);
	return (ret);
}

size_t	pack_new_cmds(char **old_arry, char **new_arry, char **ret, size_t i)
{
	size_t	o_cnt;
	size_t	n_cnt;
	size_t	j;

	j = 0;
	o_cnt = 0;
	n_cnt = 0;
	if (!old_arry || !new_arry)
		return (n_cnt);
	while (j < i)
		ret[j++] = x_strdup(old_arry[o_cnt++]);
	while (ret < new_arry[n_cnt])
		ret[j++] = x_strdup(new_arry[n_cnt++]);
	o_cnt++;
	while (ret < old_arry[o_cnt])
		ret[j++] = x_strdup(old_arry[o_cnt++]);
	ret[j] = NULL;
	return (n_cnt);
}

char	**expand_asterisk(char *line)
{
	t_cmp_str	**cmp_arry;
	t_map		*file_map;
	char		**ret;
	bool		is_dot;

	ret = NULL;
	is_dot = false;
	file_map = map_new();
	get_all_files_in_dir(file_map);
	cmp_arry = create_cmp_str_arry(line);
	if (line[0] == '.')
		is_dot = true;
	filter_map(file_map, cmp_arry, is_dot);
	ret = extract_file_name(file_map);
	//free_map(&file_map);
	return (ret);
}

void	get_all_files_in_dir(t_map *file_map)
{
	DIR				*dir;
	struct dirent	*dir_ent;
	struct stat		file_stat;

	dir = opendir(".");
	if (!dir)
		throw_system_error("expand_asterisk", "opendir failed");
	while (1)
	{
		dir_ent = readdir(dir);
		if (!dir_ent)
			break ;
		if (stat(dir_ent->d_name, &file_stat) == -1)
			continue ;
		if (S_ISDIR(file_stat.st_mode))
			map_add_item(file_map, x_strdup(dir_ent->d_name), x_strdup("DIR"));
		else
			map_add_item(file_map, x_strdup(dir_ent->d_name), x_strdup("FILE"));
	}
}

void	filter_map(t_map *file_map, t_cmp_str **cmp_arry, bool is_dot)
{
	t_item	*file;
	t_item	*next_file;

	if (!cmp_arry || !file_map)
		throw_unexpected_error("filter_map", NULL);
	file = file_map->item_head.next;
	while (file)
	{
		next_file = file->next;
		if (!match_aster_regex(file, cmp_arry, is_dot))
		{
			map_remove(file_map, file->name);
			file = NULL;
		}
		file = next_file;
	}
}

bool	match_aster_regex(t_item *file, t_cmp_str **cmp_arry, bool is_dot)
{
	size_t	i;
	size_t	j;
	char	is_match;

	is_match = true;
	if (!cmp_arry || !file)
		throw_unexpected_error("match_aster_regex", NULL);
	i = 0;
	j = 0;
	while (cmp_arry[i])
	{
		if (!j && cmp_arry[i]->cmp_type == PREFIX)
			is_match = match_prefix(file->name, &j, cmp_arry[i]->cmp_str);
		else if (cmp_arry[i]->cmp_type == SUFFIX)
			is_match = match_suffix(file, &j, cmp_arry[i]->cmp_str, is_dot);
		else if (cmp_arry[i]->cmp_type == CONTAIN)
			is_match = \
			match_contain(file->name, &j, cmp_arry[i]->cmp_str, is_dot);
		else if (cmp_arry[i]->cmp_type == ALL)
			is_match = file->name[0] != '.';
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

bool	match_suffix(t_item *f, size_t *i, char *suffix, bool is_dot)
{
	size_t	j;
	size_t	f_len;
	size_t	suffix_len;

	if (!is_dot && f->name[0] == '.')
		return (false);
	j = 0;
	f_len = ft_strlen(f->name);
	suffix_len = ft_strlen(suffix);
	if (suffix[suffix_len - 1] == '/')
		return (match_suffix_directory(f, i, suffix));
	if (*i >= f_len || f_len < suffix_len + *i)
		return (false);
	while (j <= suffix_len && f->name[f_len - j] == suffix[suffix_len - j])
	{
		if (f_len < *i + j)
			return (false);
		if (!suffix[j])
			return (true);
		j++;
	}
	*i = j;
	return (false);
}
bool	match_suffix_directory(t_item *f, size_t *i, char *suffix)
{
	size_t	j;
	size_t	f_len;
	size_t	suffix_len;

	f_len = ft_strlen(f->name);
	suffix_len = ft_strlen(suffix);
	if (ft_strcmp(f->value, "DIR"))
		return (false);
	if (suffix_len == 1)
		return (true);
	if (*i >= f_len || f_len < suffix_len + *i)
		return (false);
	j = 2;
	while (j <= suffix_len && f->name[f_len - j + 1] == suffix[suffix_len - j])
	{
		if (f_len < *i + j)
			return (false);
		if (!suffix[j])
			return (true);
		j++;
	}
	return (false);
}


bool	match_contain(char *f_name, size_t *i, char *contain, bool is_dot)
{
	size_t	k;
	size_t	contain_len;

	if (!is_dot && f_name[0] == '.')
		return (false);
	k = 0;
	contain_len = ft_strlen(contain);
	if (*i >= ft_strlen(f_name) || ft_strlen(f_name) < contain_len + *i)
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
		ret[i] = x_strdup(file->name);
		file = file->next;
		i++;
	}
	ret[i] = NULL;
	return (ret);
}
// bool	check_expandable_asterisk(char *str, size_t i)
// {
	
// }
