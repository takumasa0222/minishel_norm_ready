#include "../includes/minishell.h"

static void	envmap_init(t_map *envmap, char **ep);

// initialize a new environment map
t_map	*init_env(char *envp[])
{
	t_map	*envmap;
	envmap = map_new();
	envmap_init(envmap, envp);
	return (envmap);
}

// get the value of an environment variable
char	*xgetenv(const char *name)
{
	return (map_get(envmap, name));
}

char	**get_environ(t_map *map)
{
	size_t	i;
	size_t	size;
	t_item	*item;
	char	**environ;

	size = map_size(map, false) + 1;
	environ = malloc(sizeof(char *) * (size + 1));
	i = 0;
	item = map->item_head.next;
	while (item)
	{
		if (item->value)
		{
			environ[i] = item_get_string(item);
			i++;
		}
		item = item->next;
	}
	environ[i] = NULL;
	return (environ);
}

static void	envmap_init(t_map *envmap, char **ep)
{
	while (*ep)
	{
		map_put(envmap, *ep, false);
		ep++;
	}
}
