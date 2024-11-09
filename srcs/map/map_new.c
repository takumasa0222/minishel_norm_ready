#include "../../includes/minishell.h"
#include "../../includes/utils.h"

t_map	*map_new(void) 
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		d_throw_error("map_new", "Memory allocation error");
	map->item_head.next = NULL;
	return (map);
}
