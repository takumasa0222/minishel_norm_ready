#include "../../includes/minishell.h"

struct cmd {
	int argc;
	char **argv;
	int capa;
	int status;
	int pid;
	struct cmd *next;
};

int		invoke_commands(struct cmd *cmdhead);
void	exec_pipeline(struct cmd *cmdhead);
int		wait_pipeline(struct cmd *cmdhead);
struct cmd*	pipeline_tail(struct cmd *cmdhead);

int	main(void)
{
	char	*line;

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

int invoke_commands(struct cmd *cmdhead)
{
	int st;
	int original_stdin = dup(0);
	int original_stdout = dup(1);

	exec_pipeline(cmdhead);
	st = wait_pipeline(cmdhead);
	close(0); dup2(original_stdin, 0); close(original_stdin);
	close(1); dup2(original_stdout, 1); close(original_stdout);

	return st;
}

static void     exec_pipeline(struct cmd *cmdhead)
{
	struct cmd *cmd; //　コマンドリストをたどるポインタ 
	int fds_pre[2] = {-1, -1};
	int fds[2] = {-1, -1};

	for (cmd = cmdhead; cmd && !REDIRECT_P(cmd); cmd = cmd->next) {
		fds_pre[0] = fds[0];
		fds_pre[1] = fds[1];
		if (! TAIL_P(cmd)) { // 次のコマンドが存在する場合
			if (pipe(fds) < 0) {
				perror("pipe");
				exit(3);
			}
		} else {
			cmd->pid = fork(); // 子プロセスを生成
			if (cmd->pid < 0) {
				perror("fork");
				exit(3);
			}
			if (cmd->pid > 0) { /* parent */
				if (fds_pre[0] != -1) close(fds_pre[0]); // -1でない=有効なfdであり、リソースとして開かれている
				if (fds_pre[1] != -1) close(fds_pre[1]);
				continue;
			}
		}
		// 子プロセスはコマンド実行
		// fd[0]read_endは入力を受け取る場所
		if (! HEAD_P(cmd)) { // 先頭のコマンド以外、fds[0]をstdinに設定し、fds_preを閉じる
			close(STDIN_FILENO); // close stdin
			dup2(fds_pre[0], STDIN_FILENO); // fds_pre[0] read_end をstdinにリダイレクト
			close(fds_pre[0]); // already duped, close it
			close(fds_pre[1]); // child process does not need the write end of the pipe
		}
		// 
		if (! TAIL_P(cmd)) { // 最後のコマンド以外、fds[1]をstdoutに設定し、fdsを閉じる
			close(fds[0]); // read end is read by the previous command
			close(STDOUT_FILENO); // close stdout
			dup2(fds[1], STDOUT_FILENO); // fds[1] write_end をstdoutにリダイレクト
			close(fds[1]); // already duped, close it
		}
		execvp(cmd->argv[0], cmd->argv);
		printf("command not found: %s\n", cmd->argv[0]);
		exit(EXIT_FAILURE);
	}
}

int		wait_pipeline(struct cmd *cmdhead)
{
	struct cmd *cmd;

	for (cmd = cmdhead; cmd && !REDIRECT_P(cmd); cmd = cmd->next) {
		waitpid(cmd->pid, &cmd->status, 0);
	}
	return pipeline_tail(cmdhead)->status;
}

struct cmd* pipeline_tail(struct cmd *cmdhead)
{
    struct cmd *cmd;

    for (cmd = cmdhead; !TAIL_P(cmd); cmd = cmd->next)
        ;
    return cmd;
}