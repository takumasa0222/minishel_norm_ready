#include "../includes/minishell.h"

// initialize a new environment map
t_map	*initenv(char *envp[])
{
	t_map	*envmap;
	envmap = map_new();
	envmap_init(envmap, envp);
	return (envmap);
}

// get the value of an environment variable
char *xgetenv(t_map *envmap, const char *name)
{
    return (map_get(envmap, name));
}

char **get_environ(t_map *envmap)
{
    int i;
    size_t size = map_size(envmap, true);
    char **envp = malloc(sizeof(char *) * (size + 1));
    if (!envp)
        throw_err("get_environ", "Memory allocation error");
    t_item *item = envmap->item_head.next;

    i = 0;
    while (i < size)
    {
        if (item->value)
        {
            envp[i] = item_get_string(item);
            item = item->next;
        }
        i++;
    }
    envp[size] = NULL;
    return envp;
}

void init_path(char **envp, t_path_node **path_var)
{
    int i = 0;
    char *tmp = NULL;

    if (!path_var)
        d_throw_error("init_path", "no path_var");
	exit(EXIT_FAILURE);

    if (!envp)
        return;

    while (envp[i]) {
        if (!ft_strncmp(envp[i], "PATH=", 5)) {
            tmp = ft_substr(envp[i], 5, ft_strlen(envp[i]));  // "PATH="以降の文字列
            char **dirs = ft_split(tmp, ':');  // `:`で分割

            free(tmp);

            // 連結リストに変換
            int j = 0;
            while (dirs[j]) {
                t_path_node *new_node = malloc(sizeof(t_path_node));
                if (!new_node)
                    throw_err("Memory allocation error", BASH_GENERAL_ERR);
                new_node->dir = dirs[j];
                new_node->next = *path_var;
                *path_var = new_node;
                j++;
            }
            free(dirs);
            return;
        }
        i++;
    }
}
