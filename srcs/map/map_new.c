/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_new.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/01/16 21:39:01 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"

/*
item_head is a dummy item which is not counted in the size of the map
item_head is not a pointer 
so its memory is automatically allocated when t_map is created
malloc(sizeof(t_map))
(if item_head is a pointer, we need to allocate memory for it)
sizeof(t_map) = sizeof(t_item)
	char *name: 8 byte
	char *value: 8 byte
	struct s_item *next: 8 byte
total 24 byte
if item_head is a pointer, 8 byte needs to be allocated
map->item_head.next is NULL at the beginning 
*/
t_map	*map_new(void)
{
	t_map	*map;

	map = xmalloc(sizeof(t_map));
	map->item_head.next = NULL;
	return (map);
}
