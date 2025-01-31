/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_remove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 05:15:56 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 05:18:20 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/map.h"
#include <stdlib.h>

int	map_remove(t_map *map, const char *name)
{
	t_item	*cur;
	t_item	*prev;

	if (!map || name == NULL)
		return (EXIT_FAILURE);
	prev = &map->item_head;
	cur = map->item_head.next;
	while (cur)
	{
		if (ft_strcmp_for_map(cur->name, name) == 0)
		{
			prev->next = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			return (EXIT_SUCCESS);
		}
		prev = cur;
		cur = cur->next;
	}
	return (EXIT_SUCCESS);
}
