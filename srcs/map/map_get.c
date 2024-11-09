#include "../../includes/minishell.h"

char	*map_get(t_map *map, const char *name)
{
	t_item	*cur;
	size_t	name_len;
	size_t	cur_name_len;

	name_len = ft_strlen(name);
	if (name == NULL)
		return (NULL);
	cur = map->item_head.next;
	while (cur)
	{
		cur_name_len = ft_strlen(cur->name);
		if (ft_strncmp(cur->name, name, name_len) == 0 && cur_name_len == name_len)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}

// cur_name_len == name_len is added
// to prevent the case where the name is a substring of the item name
// for example, if the name is "HOME" and the item name is "HOME2",