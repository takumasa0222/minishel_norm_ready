/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_tester.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 22:08:43 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/10 20:36:53 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

typedef struct
{
	const char	*cmd;
	int			argc;
	char		*argv[3];
	const char	*description;
}				TestCase;

void			test_builtin_no_envmap(const char *cmd, int argc, char *argv[]);
void			test_builtin_with_envmap(const char *cmd, int argc, char *argv[], t_map *envmap);
void			test_envmap_operations(void);

TestCase		no_envmap_test_cases[] = {
	{"cd", 2, {"cd", "/tmp", NULL}, "cd to /tmp"},
	{"pwd", 1, {"pwd", NULL}, "pwd (expect /tmp)"},
	{"cd", 2, {"cd", "/non-existent-dir", NULL},
		"Attempt to change to a non-existent directory"},
	{"pwd", 1, {"pwd", NULL}, "Check current directory after failed cd"},
	{"cd", 2, {"cd", "..", NULL}, "Move to the parent directory of /tmp"},
	{"pwd", 1, {"pwd", NULL}, "cd after moving up a level (expect /)"}
};

TestCase		with_envmap_test_cases[] = {
	{"export", 2, {"export", "NEW_VAR=new_value", NULL}, "Export NEW_VAR with value"},
	{"export", 2, {"export", "NEW_VAR=updated_value", NULL}, "Update NEW_VAR with new value"},
	{"export", 2, {"export", "1INVALID_NAME", NULL}, "Attempt to export an invalid identifier"},
	{"export", 1, {"export", NULL}, "Print all environment variables"}
};

int	main(void)
{
	printf("Testing builtins without envmap...\n");
	for (size_t i = 0; i < sizeof(no_envmap_test_cases) / sizeof(TestCase); i++)
	{
		printf("Case %zu: %s\n", i + 1, no_envmap_test_cases[i].description);
		test_builtin_no_envmap(no_envmap_test_cases[i].cmd, no_envmap_test_cases[i].argc, no_envmap_test_cases[i].argv);
	}
	printf("Non-envmap builtins test done.\n");

	printf("Testing envmap operations...\n");
	test_envmap_operations();
	printf("Envmap operations test done.\n");

	exit(0);
}

// test for cd and pwd
void	test_builtin_no_envmap(const char *cmd, int argc, char *argv[])
{
	t_builtin	*builtin;

	builtin = lookup_builtin((char *)cmd);
	if (builtin)
	{
		int status = builtin->f(argc, argv, NULL);
		if (status != 0)
			printf("%s execution failed with status %d.\n", cmd, status);
	}
	else
	{
		printf("%s is not a builtin command.\n", cmd);
	}
}

// test for export and other builtins with envmap
void	test_builtin_with_envmap(const char *cmd, int argc, char *argv[], t_map *envmap)
{
	t_builtin	*builtin;

	builtin = lookup_builtin((char *)cmd);
	if (builtin)
	{
		int status = builtin->f(argc, argv, envmap);
		if (status != 0)
			printf("%s execution failed with status %d.\n", cmd, status);
	}
	else
	{
		printf("%s is not a builtin command.\n", cmd);
	}
}

// envmapの初期化、追加・更新・削除のテスト
void	test_envmap_operations(void)
{
	char		*envp[] = {"HOME=/home/user", "PATH=/usr/bin", "SHELL=/bin/bash", NULL};
	t_map		*envmap = init_env(envp);

	if (!envmap)
	{
		printf("Failed to initialize environment map.\n");
		return;
	}

	printf("Environment map initialized.\n");
	const char *initial_vars[] = {"PATH", "HOME", "SHELL"};
	for (size_t i = 0; i < sizeof(initial_vars) / sizeof(initial_vars[0]); i++)
	{
		const char *value = map_get(envmap, initial_vars[i]);
		if (value)
			printf("%s=%s\n", initial_vars[i], value);
		else
			printf("Failed to get %s from envmap.\n", initial_vars[i]);
	}

	// 追加と更新のテスト
	for (size_t i = 0; i < sizeof(with_envmap_test_cases) / sizeof(TestCase); i++)
	{
		printf("Case %zu: %s\n", i + 1, with_envmap_test_cases[i].description);
		test_builtin_with_envmap(with_envmap_test_cases[i].cmd, with_envmap_test_cases[i].argc, with_envmap_test_cases[i].argv, envmap);
	}

	// 環境変数の表示
	printf("Environment variables after operations:\n");
	char **environ = get_environ(envmap);
	for (size_t i = 0; environ && environ[i] != NULL; i++)
	{
		printf("%s\n", environ[i]);
		free(environ[i]);
	}
	free(environ);

	// 環境変数の削除
	printf("Unsetting all environment variables...\n");
	for (size_t i = 0; i < sizeof(initial_vars) / sizeof(initial_vars[0]); i++)
	{
		if (map_unset(envmap, initial_vars[i]) != 0)
		{
			printf("Failed to unset %s\n", initial_vars[i]);
		}
	}
	map_unset(envmap, "NEW_VAR"); 
	// すべての環境変数が削除されたことを確認
	char **empty_environ = get_environ(envmap);
	if (empty_environ[0] == NULL)
		printf("All environment variables are unset.\n");
	else
		printf("Failed to unset all environment variables.\n");
	for (size_t i = 0; empty_environ[i] != NULL; i++)
		free(empty_environ[i]);
	free(empty_environ);
	free(envmap);
}

