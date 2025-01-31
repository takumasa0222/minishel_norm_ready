/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_recreate_strings.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:20:25 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 17:23:28 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"
#include "../../includes/expand_asterisk.h"

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
	new_cmds = xmalloc(sizeof(char *) * (origin_arry_size + new_arry_size));
	ret = pack_new_cmds(node->cmds, file_arry, new_cmds, i);
	free_wordlist(&node->cmds);
	free_wordlist(&file_arry);
	node->cmds = NULL;
	node->cmds = new_cmds;
	return (ret);
}

size_t	pack_new_cmds(char **old_arry, char **f_arry, char **ret, size_t i)
{
	size_t	o_cnt;
	size_t	n_cnt;
	size_t	j;

	j = 0;
	o_cnt = 0;
	n_cnt = 0;
	if (!old_arry || !f_arry)
		return (n_cnt);
	while (j < i)
		ret[j++] = x_strdup(old_arry[o_cnt++]);
	while (f_arry[n_cnt])
		ret[j++] = x_strdup(f_arry[n_cnt++]);
	o_cnt++;
	while (old_arry[o_cnt])
		ret[j++] = x_strdup(old_arry[o_cnt++]);
	ret[j] = NULL;
	return (n_cnt);
}
