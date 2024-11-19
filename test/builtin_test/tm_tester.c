#include "../../includes/minishell.h"
#include "../../includes/builtin.h"


void	test_builtin_echo(int argc,  char **envp);
t_map 	*test_envmap_operations(void);
void	test_builtin_export(int argc,  char **envp);
void	test_builtin_unset(void);

int	main (int argc, char *argv[], char **envp)
{
	printf("%s %d, %s\n",argv[0], argc, envp[0]);
	//test_builtin_echo(argc, envp);
	//test_builtin_export(argc, envp);
	test_builtin_unset();
}

void	test_builtin_echo(int argc,  char **envp)
{
	(void)envp;
	char	*test_cases[][5] = {{"echo", NULL},\
								{"echo","case1", NULL}, \
								{"echo","-n", "case2", NULL}, \
								{"echo","-n-", "case3", NULL}, \
								{"echo","case4","test","test", NULL}, \
								};
	int	i;

	i = 0;
	while (i < 5)
	{
		printf("test %d\n",i);
		builtin_echo(argc, test_cases[i], NULL);
		printf("test %d done\n",i);
		i++;
	}
}

void	test_builtin_export(int argc,  char **envp)
{
	(void)envp;
	t_map	*envmap;

	char	*test_cases[][5] = {{"export", NULL},\
								{"export","case1=test", NULL}, \
								{"export","case2=test2","case2=pattern2", NULL}, \
								{"export", NULL}, \
								{"export","case4", NULL}, \
								{"export", NULL},
								};
	int	i;

	envmap = test_envmap_operations();
	i = 0;
	while (i < 6)
	{
		printf("test %d\n",i);
		builtin_export(argc, test_cases[i], envmap);
		printf("test %d done\n",i);
		i++;
	}
}

t_map *test_envmap_operations(void)
{
	char		*envp[] = {"HOME=/home/ssoeno", "PATH=/usr/bin", "SHELL=/bin/bash", NULL};
	t_map		*envmap = init_env(envp);

	return (envmap);
}

void	test_builtin_unset()
{
	char		*envp[] = {"HOME=/home/ssoeno", "PATH=/usr/bin", "SHELL=/bin/bash", NULL};
	t_map		*envmap = init_env(envp);
	int i;

	i = 0;
	char	*test_cases[][5] = {{"unset", NULL},\
								{"unset","HOME", NULL}, \
								{"unset","PATH","SHELL", NULL}, \
								{"unset", NULL}, \
								{"unset","abc", NULL}, 
								{"unset","case6", NULL}
								};
	char	*test_cases2[3] = 	{"export", NULL};

	while (i < 5)
	{
		printf("test %d\n",i);
		builtin_unset(0, test_cases[i], envmap);
		builtin_export(1, &test_cases2[0], envmap);
		printf("test %d done\n",i);
		i++;
	}
	printf("test %d\n",i);
	builtin_unset(0, test_cases[i], NULL);
	printf("test %d done\n",i);
}
