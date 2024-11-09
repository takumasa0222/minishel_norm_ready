#include "../../includes/minishell.h"

char	*map_get(t_map *map, const char *name)
{
	t_item	*cur;

	if (name == NULL)
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
