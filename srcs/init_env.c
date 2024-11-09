#include "../includes/minishell.h"

static void	envmap_init(t_map *envmap, char **ep);

// wrapper for map_get to get environment variables
char *xgetenv(t_map *envmap, const char *name)
{
    return map_get(envmap, name);
}

// initialize a new environment map
t_map	*init_env(char *envp[])
{
	t_map	*envmap;
	envmap = map_new();
	envmap_init(envmap, envp);
	return (envmap);
}

// char **ep is formatted as "KEY=VALUE"
// map_put() to add each environment variable to t_map
static void	envmap_init(t_map *envmap, char **ep)
{
	while (*ep)
	{
		map_put(envmap, *ep, false);
		ep++;
	}
}

// get each environment variable from t_map
// and generate an environment variable array ("KEY=VALUE")
// the last element of the array is NULL
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

