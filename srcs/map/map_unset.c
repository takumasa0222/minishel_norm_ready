#include "../../includes/minishell.h"

int	map_unset(t_map *map, const char *name)
{
	t_item	*cur;
	t_item	*prev;
	size_t	name_len;
	size_t	cur_name_len;

	name_len = ft_strlen(name);
	if (name == NULL || !is_identifier(name))
		return (-1);
	prev = &map->item_head;
	cur = map->item_head.next;
	while (cur)
	{
		cur_name_len = ft_strlen(cur->name);
		if (ft_strncmp(cur->name, name, name_len) == 0 && cur_name_len == name_len)
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
