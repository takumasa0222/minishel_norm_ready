#include "../../includes/minishell.h"

int	map_unset(t_map *map, const char *name)
{
	t_item	*cur;
	t_item	*prev;

	if (name == NULL || !is_identifier(name))
		return (-1);
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
// return -1 if the name is NULL or not a valid identifier
//  (throw errorにすべき？あとで相談）
// if the name is in the map, remove it and
// free the memory of the item (name, value, item itself)
