/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:46 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/10/27 17:07:41 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*line;

	// test for builtins
	printf("Testing builtins...\n");
	printf("Case 1: cd to /tmp and display the directory\n");
	char *cd_args1[] = {"cd", "/tmp"}; 
	test_builtin("cd", 2, cd_args1);
	char *pwd_args1[] = {"pwd"};
	test_builtin("pwd", 1, pwd_args1);
	printf("Case 2: cd to non-existent directory\n");
	char *cd_args2[] = {"cd", "/non-existent-dir"};
	test_builtin("cd", 2, cd_args2);
	char *pwd_args2[] = {"pwd"};
	test_builtin("pwd", 1, pwd_args2);
	printf("Case 3: Move to the directory one level above the /tmp directory.\n");
	char *cd_args3[] = {"cd", ".."};
	test_builtin("cd", 2, cd_args3);
	char *pwd_args3[] = {"pwd"};
	test_builtin("pwd", 1, pwd_args3);
	
	// char *exit_args[] = {"exit"};
	// test_builtin("exit", 1, exit_args);
	printf("Builtins test done.\n");

	rl_outstream = stderr;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
			// TODO: intepret line as a command
		free(line);
	}
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

t_builtin	*lookup_builtin(char *cmd)
{
	t_builtin	builtins_list[4];
	t_builtin	*p;

	builtins_list[0] = (t_builtin){"cd", builtin_cd};
	builtins_list[1] = (t_builtin){"pwd", builtin_pwd};
	builtins_list[2] = (t_builtin){"exit", builtin_exit};
	builtins_list[3] = (t_builtin){NULL, NULL};
	p = builtins_list;
	while (p->name)
	{
		if (ft_strncmp(cmd, p->name, ft_strlen(p->name)) == 0)
			return (p);
		p++;
	}
	return (NULL);
}

int	builtin_cd(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("%s: wrong argument\n", argv[0]);
		return (1);
	}
	if (chdir(argv[1]) < 0)
	{
		perror(argv[1]);
		return (1);
	}
	return (0);
}

int	builtin_pwd(int argc, char *argv[])
{
	char	buf[PATH_MAX];

	if (argc != 1)
	{
		printf("%s: wrong argument\n", argv[0]);
		return (1);
	}
	if (!getcwd(buf, PATH_MAX))
	{
		printf("%s: cannot get working directory\n", argv[0]);
		return (1);
	}
	printf("%s\n", buf);
	return (0);
}

int	builtin_exit(int argc, char *argv[])
{
	if (argc != 1)
	{
		printf("%s: too many arguments\n", argv[0]);
		return (1);
	}
	exit(0);
}
