#include "../../includes/minishell.h"

int split_name_value(const char *string, 
		bool allow_empty_value, char **name, char **value)
{
	char    *name_end;

	name_end = ft_strchr(string, '=');
	if (name_end == NULL)
	{
		if (!allow_empty_value)
			return (-1);
		*name = ft_strdup(string);
		*value = NULL;
	}
	else
	{
		*name = ft_substr(string, 0, name_end - string);
		*value = ft_strdup(name_end + 1);
	}
	return (EXIT_SUCCESS);
}

int	map_put(t_map *map, const char *string, bool allow_empty_value)
{
	int		result;
	char	*name;
	char	*value;

	name = NULL;
	value = NULL;
	result = split_name_value(string, allow_empty_value, &name, &value);
	if (result != 0)
		return (result);
	result = map_set(map, name, value);
	free(name);
	free(value);
	return (result);
}

/*
initialize a new environment map
t_map	*init_env(char *envp[])
{
	t_map	*envmap;
	envmap = map_new();
	envmap_init(envmap, envp);
	return (envmap);
}

char **ep is formatted as "KEY=VALUE"
map_put() to add each environment variable to t_map
static void	envmap_init(t_map *envmap, char **ep)
{
	while (*ep)
	{
		map_put(envmap, *ep, false);
		ep++;
	}
}
*/