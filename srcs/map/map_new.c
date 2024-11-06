#include "../../includes/minishell.h"

t_map	*map_new(void) 
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		throw_err("map_new", "Memory allocation error");
	map->item_head.next = NULL;
	return (map);
}
