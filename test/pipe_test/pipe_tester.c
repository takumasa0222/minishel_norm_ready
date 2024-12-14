#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/pipe.h"

struct cmds *create_test_commands()
{
    struct cmds *cmd1 = malloc(sizeof(struct cmds));
    struct cmds *cmd2 = malloc(sizeof(struct cmds));
    
    if (!cmd1 || !cmd2)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // cmd1 (echo hello)
    cmd1->argc = 2;
    cmd1->cmd = malloc(3 * sizeof(char *));
    cmd1->cmd[0] = strdup("echo");
    if (!cmd1->cmd[0])
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    cmd1->cmd[1] = strdup("hello");
    if (!cmd1->cmd[1])
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    cmd1->cmd[2] = NULL;
    cmd1->exit_status = 0;
    cmd1->next = cmd2;
    // cmd2 (wc -l)
    cmd2->argc = 2;
    cmd2->cmd = malloc(3 * sizeof(char *));
    cmd2->cmd[0] = strdup("wc");
    if (!cmd2->cmd[0])
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    cmd2->cmd[1] = strdup("-l");
    if (!cmd2->cmd[1])
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
	cmd2->cmd[2] = NULL;
    cmd2->exit_status = 0;
    cmd2->next = NULL;

    return cmd1;
}

void free_commands(struct cmds *cmdhead)
{
    struct cmds *tmp;
    while (cmdhead)
    {
        tmp = cmdhead;
        cmdhead = cmdhead->next;
        int i = 0;
        while (i < tmp->argc)
            free(tmp->cmd[i++]);
        free(tmp->cmd);
        free(tmp);
    }
}

int	main(void)
{
	int exit_status;
	struct cmds *cmdhead;
	
	cmdhead = create_test_commands();
	exit_status = invoke_commands(cmdhead);
    if (exit_status == -1) {
        fprintf(stderr, "Error: Command execution failed\n");
        free_commands(cmdhead);
        return (EXIT_FAILURE);
    }
	printf("test done...exit status: %d\n", exit_status);

	// free after use
	struct cmds *tmp;
	while (cmdhead)
	{
		tmp = cmdhead;
		cmdhead = cmdhead->next;
		int i = 0;
		while (i < tmp->argc)
			free(tmp->cmd[i++]);
		free(tmp->cmd);
		free(tmp);
	}
	return (0);
}
