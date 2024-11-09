/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_tester.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 22:08:43 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/09 14:06:24 by ssoeno           ###   ########.fr       */
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

TestCase test_cases[] = {
	{"cd", 2, {"cd", "/tmp", NULL}, "cd to /tmp"},
	{"pwd", 1, {"pwd", NULL}, "pwd (expect /tmp)"},
	{"cd", 2, {"cd", "/non-existent-dir", NULL}, "Attempt to change to a non-existent directory"},
	{"pwd", 1, {"pwd", NULL}, "Check current directory after failed cd"},
	{"cd", 2, {"cd", "..", NULL}, "Move to the parent directory of /tmp"},
	{"pwd", 1, {"pwd", NULL}, "cd after moving up a level (expect /)"},
	// exitはプログラムが終了してしまうので、別の方法でのテストを考える
};

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	t_map *envmap = init_env(envp);
	printf("Testing builtins...\n");

	char **environ = get_environ(envmap);
	if (environ == NULL)
	{
		printf("Failed to get environment variables.\n");
		exit(EXIT_FAILURE);
	}
	printf("initialize environment variables:\n");
	for (size_t i = 0; environ[i] != NULL; i++)
	{
		printf("%s\n", environ[i]);
	}

	// test for map_unset
	for (int i = 0; environ[i] != NULL; i++)
	{
		char *key = strtok(environ[i], "=");
		if (key)
		{
			printf("Unsetting %s\n", key);
			if (map_unset(envmap, key) != 0)
			{
				printf("Failed to unset %s\n", key);
			}
		}
	}
	char **empty_environ = get_environ(envmap);
	if (empty_environ[0] == NULL)
	{
		printf("all environment variables are unset.\n");
	} else {
		printf("Failed to unset all environment variables.\n");
	}
	free(environ);
	free(empty_environ);

	for (size_t i = 0; i < sizeof(test_cases) / sizeof(TestCase); i++)
	{
		printf("Case %zu: %s\n", i + 1, test_cases[i].description);
		test_builtin(test_cases[i].cmd, test_cases[i].argc, test_cases[i].argv);
	}
	printf("Builtins test done.\n");
	exit(0);
}

// test for map
// void	test_map(t_map *map)
// {
// 	map_printall(map);
// }

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
