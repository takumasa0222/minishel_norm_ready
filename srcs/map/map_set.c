#include "../../includes/minishell.h"

bool	is_identifier(const char *s)
{
	if (!ft_isalpha(*s) && *s != '_')
		return (false);
	s++;
	while (*s)
	{
		if (!ft_isalnum(*s) && *s != '_')
			return (false);
		s++;
	}
}
// to check if the string follows the rules of an identifier 
// (starting with an alphabet or underscore, 
// followed by alphabet, digits, or underscores)"

t_item	*map_find_item(t_map *map, const char *name)
{
	t_item	*cur;

	cur = map->item_head.next;
	while(cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
			return cur;
		cur = cur->next;
	}
	return (NULL);
}

void	map_update_item(t_item *item, const char *value)
{
	free(item->value);
	if (value == NULL)
		item->value = NULL;
	else
	{
		item->value = ft_strdup(value);
		if (item->value == NULL)
			throw_err("map_update_item", "Memory allocation error");
	}
}

void	map_add_item(t_map *map, const char *name, const char *value)
{
	t_item	*new_item;

	if (value == NULL)
	{
		new_item = item_new(strdup(name), NULL);
		if (new_item->name == NULL)
			throw_err("map_set", "Memory allocation error");
	}
	else
	{
		new_item = item_new(ft_strdup(name), ft_strdup(value));
		if (new_item->name == NULL || new_item->value == NULL)
			throw_err("map_set", "Memory allocation error");
	}
	new_item->next = map->item_head.next;
	map->item_head.next = new_item;
}

int map_set(t_map *map, const char *name, const char *value)
{
	t_item	*item;

	if (name == NULL || !is_identifier(name))
		return (-1);
	item = map_find_item(map, name);
	if (item)
		map_update_item(item, value);
	else
		map_add_item(map, name, value);
	return (EXIT_SUCCESS);
}