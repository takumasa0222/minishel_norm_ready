/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_tester.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 22:08:43 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/09 21:52:44 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

typedef struct {
	const char	*cmd;
	int argc;
	char *argv[3];
	const char	*description;
} TestCase;

void	test_builtin(const char *cmd, int argc, char *argv[]);
void	test_map(void);

TestCase test_cases[] = {
	{"cd", 2, {"cd", "/tmp", NULL}, "cd to /tmp"},
	{"pwd", 1, {"pwd", NULL}, "pwd (expect /tmp)"},
	{"cd", 2, {"cd", "/non-existent-dir", NULL}, "Attempt to change to a non-existent directory"},
	{"pwd", 1, {"pwd", NULL}, "Check current directory after failed cd"},
	{"cd", 2, {"cd", "..", NULL}, "Move to the parent directory of /tmp"},
	{"pwd", 1, {"pwd", NULL}, "cd after moving up a level (expect /)"},
	// exitはプログラムが終了してしまうので、別の方法でのテストを考える
};

int	main(void)
{
	test_map();

	printf("Testing builtins...\n");

	for (size_t i = 0; i < sizeof(test_cases) / sizeof(TestCase); i++)
	{
		printf("Case %zu: %s\n", i + 1, test_cases[i].description);
		test_builtin(test_cases[i].cmd, test_cases[i].argc, test_cases[i].argv);
	}
	printf("Builtins test done.\n");
	exit(0);
}

// test for builtin
void	test_builtin(const char *cmd, int argc, char *argv[])
{
	t_builtin	*builtin;
	int			status;

	builtin = lookup_builtin((char *)cmd);
	if (builtin)
	{
		status = builtin->f(argc, argv);
		if (status == 0)
			;
		else
			printf("%s execution failed with status %d.\n", cmd, status);
	}
	else
	{
		printf("%s is not a builtin command.\n", cmd);
	}
}

void	test_map()
{
	char *envp[] = {
		"HOME=/home/user",
		"PATH=/usr/bin",
		"SHELL=/bin/bash",
		NULL
	};
	
	t_map	*envmap = init_env(envp);
	if (!envmap)
	{
		printf("Failed to initialize environment map.\n");
		return ;
	}

	const char *path_value = map_get(envmap, "PATH");
	const char *home_value = map_get(envmap, "HOME");
	const char *shell_value = map_get(envmap, "SHELL");
	if (path_value && home_value && shell_value)
	{
		printf("Environment map initialized.\n");
		printf("PATH=%s\n", path_value);
		printf("HOME=%s\n", home_value);
		printf("SHELL=%s\n", shell_value);
	}
	else
	{
		printf("Failed to get environment variables.\n");
	}

	// test map_put
	printf("map_put test\n");
	if (map_put(envmap, "NEW_VAR=new_value", false) == 0)
	{
		const char *new_var_value = map_get(envmap, "NEW_VAR");
		if (new_var_value && ft_strcmp_for_map(new_var_value, "new_value") == 0)
		{
			printf("map_put: NEW_VAR=new_value added with value '%s'\n", new_var_value);
		}
		else
		{
			printf("map_put failed.\n");
		}
	}
	if (map_put(envmap, "NEW_VAR=new_value", false) == 0)
	{
		const char *new_var_value = map_get(envmap, "NEW_VAR");
		printf("DEBUG: After map_put, NEW_VAR = %s\n", new_var_value);
	}

	// test map_set
	printf("map_set test\n");
	if (map_set(envmap, "NEW_VAR", "updated_value") == 0)
	{
		const char *updated = map_get(envmap, "NEW_VAR");
		if (updated && ft_strcmp_for_map(updated, "updated_value") == 0)
		{
			printf("map_set: NEW_VAR updated with value '%s'\n", updated);
		}
		else
		{
			printf("map_set failed.\n");
		}
	}
	
	// get_environ test
	printf("get_environ test\n");
	char **environ = get_environ(envmap);
	if (environ == NULL)
	{
		printf("Failed to get environment variables.\n");
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; environ[i] != NULL; i++)
	{
		printf("%s\n", environ[i]);
	}

	// map_unset test
	printf("map_unset test\n");
	for (int i = 0; environ[i] != NULL; i++)
	{
		char *env_copy = ft_strdup(environ[i]);
		char *key = strtok(env_copy, "=");
		if (key)
		{
			printf("Unsetting %s\n", key);
			if (map_unset(envmap, key) != 0)
			{
				printf("Failed to unset %s\n", key);
			}
		}
		free(env_copy);
	}
	// check if all environment variables are unset
	char **empty_environ = get_environ(envmap);
	if (empty_environ[0] == NULL)
	{
		printf("all environment variables are unset.\n");
	} else {
		printf("Failed to unset all environment variables.\n");
	}
	// get_environで取得した配列と各要素を解放
    for (size_t i = 0; environ[i] != NULL; i++)
    {
        free(environ[i]);
    }
    free(environ);
    for (size_t i = 0; empty_environ[i] != NULL; i++)
    {
        free(empty_environ[i]);
    }
    free(empty_environ);
	free(envmap);
}

