/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_size.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:54:10 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/17 19:54:13 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtin.h"

size_t	map_size(t_map *map, bool count_null_value)
{
	size_t	size;
	t_item	*item;

	size = 0;
	item = map->item_head.next;
	while (item)
	{
		if (count_null_value || item->value)
			size++;
		item = item->next;
	}
	return (size);
}
