#include "../../includes/minishell.h"
#include "../../includes/builtin.h"


void	test_builtin_echo(int argc,  char **envp);

int	main (int argc, char *argv[], char **envp)
{
	printf("%s\n",argv[0]);
	test_builtin_echo(argc, envp);
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