/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:53:27 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/11 11:51:44 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtin.h"

char	*map_get(t_map *map, const char *name)
{
	t_item	*cur;

	if (!map || !name)
		return (NULL);
	cur = map->item_head.next;
	while (cur)
	{
		if (ft_strcmp_for_map(cur->name, name) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}
