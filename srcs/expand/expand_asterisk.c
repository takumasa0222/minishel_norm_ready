/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:23:01 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 17:26:06 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"
#include "../../includes/expand_asterisk.h"

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
	free_cmp_arry(&cmp_arry);
	ret = extract_file_name(file_map);
	free_map(&file_map);
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
			map_add_item(file_map, dir_ent->d_name, "DIR");
		else
			map_add_item(file_map, dir_ent->d_name, "FILE");
	}
	free(dir);
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
