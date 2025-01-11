/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/11 16:59:48 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"
#include <unistd.h>
#include "../includes/redirect.h"

void	set_redirect_fds(t_node *node, t_context *ctx)
{
	// int	last_input_fd; 
	// int	last_output_fd;

	// last_input_fd = -1;
	// last_output_fd = -1;
	// if (node->left)
	// {
	// 	open_files();
	// 	last_input_fd = retrieve_last_input_fd(node->left->redirects, last_input_fd);
	// 	last_output_fd = retrieve_last_output_fd(node->left->redirects, last_output_fd);
	// }

	// this is temporary implementation
	// if (!ctx->is_exec_in_child_ps)
	// 	store_std_in_out();

	printf("DEBUG set_redirect_fds\n");

	int	i;

	i = 0;
	ctx->stored_stdin = dup(STDIN_FILENO);
	ctx->stored_stdout = dup(STDOUT_FILENO);
	if (ctx->stored_stdin < 0 || ctx->stored_stdout < 0)
		d_throw_error("set_redirect_fds", "dup failed");
	while (node->redirects[i])
	{
		char *op = node->redirects[i];
		char *filename = node->redirects[i + 1];
		int fd;
		if (!filename)
			d_throw_error("set_redirect_fds", "filename is NULL");
		if (ft_strcmp(op, ">") == 0)
		{
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				d_throw_error("set_redirect_fds", "open failed");
			if (dup2(fd, STDOUT_FILENO) < 0)
				d_throw_error("set_redirect_fds", "dup2 failed");
			close(fd);
		}
		else if (ft_strcmp(op, ">>") == 0)
		{
			fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				d_throw_error("set_redirect_fds", "open failed");
			if (dup2(fd, STDOUT_FILENO) < 0)
				d_throw_error("set_redirect_fds", "dup2 failed");
			close(fd);
		}	
		else if (ft_strcmp(op, "<") == 0)
		{
			fd = open(filename, O_RDONLY);
			if (fd < 0)
				d_throw_error("set_redirect_fds", "open failed");
			if (dup2(fd, STDIN_FILENO) < 0)
				d_throw_error("set_redirect_fds", "dup2 failed");
			close(fd);			
		}
		if (node->fd_num < 0)
			d_throw_error("set_redirect_fds", "open failed");
		i += 2;
	}
}


void restore_std_fds(t_context *ctx)
{
	if (ctx->stored_stdin >= 0)
	{
		if (dup2(ctx->stored_stdin, STDIN_FILENO) < 0)
			d_throw_error("restore_std_fds", "dup2 failed");
		close(ctx->stored_stdin);
		ctx->stored_stdin = -1;
	}
	if (ctx->stored_stdout >= 0)
	{
		if (dup2(ctx->stored_stdout, STDOUT_FILENO) < 0)
			d_throw_error("restore_std_fds", "dup2 failed");
		close(ctx->stored_stdout);
		ctx->stored_stdout = -1;
	}
}

/*
- pre_in_pipe_fd: 前のパイプのread endを保存。次のコマンドのstdin（標準入力）として使う
- in_pipe_fd: 次に作ったパイプのread end
- out_pipe_fd: 次に作ったパイプのwrite end
新たにパイプを作ったら、この2つに書き込み口・読み取り口を保存しておく。
コマンドを実行するときには、
stdinをin_pipe_fd（またはpre_in_pipe_fd）に差し替え、
stdoutをout_pipe_fdに差し替える

例1: cat1 Makefile | cat2 | cat3
[cat1 Makefile]のstdoutをout_pipe_fd(pipe1のwrite end)に差し替える。
	1つ目のコマンドなので入力は標準入力
[cat2]　今までin_pipe_fdに入っていたパイプの読み取り口を、pre_in_pipe_fdへ移動。
	これで「前のコマンドの出力を受け取るパイプの読み取り口」をpre_in_pipe_fdで管理できる。
	次のパイプを作り、in_pipe_fd, out_pipe_fdを更新。
	stdinをpre_in_pipe_fdに差し替えることで、cat2は前のコマンド（cat1）からの出力を読む。
	stdoutを新しいout_pipe_fdに差し替えることで、cat2の出力がさらに次のパイプへ流れる。
[cat3]　
	同様にpre_in_pipe_fdをin_pipe_fdに移し、cat3のstdinをそのpre_in_pipe_fdに差し替える。
	最後のコマンドなので出力は標準出力のまま

例2: cat file2 > out | cat
	in_pipe_fd, out_pipe_fdがそれぞれパイプの“読み取り口”“書き込み口”になるようにpipe()で作る。
	まずout_pipe_fdにstdoutを差し替えようとする
	さらにリダイレクトで stdout を out ファイルに差し替える
	すると、パイプ用に確保したout_pipe_fdはもう不要になるので閉じる（close(out_pipe_fd)）
	結局、cat file2 の出力はファイルに書き込まれる。終わったら、STDOUT や STDIN を元に戻す。
	次のコマンド (cat)
	パイプに書き込んでいないので、中身（file2の内容）は実際には次のパイプに流れない
	（outファイルに書き込んだから）。
	もしパイプを作ったままなら、pre_in_pipe_fd を STDIN に差し替えても、
	書き込み先がファイルだったのでパイプの中にはデータが無い。
	その結果、次の cat は何もデータを受け取れない。

例3: cat file2 | cat
	パイプ生成でin_pipe_fd と out_pipe_fd を用意
	stdout を out_pipe_fd に差し替え
		cat file2 の出力をパイプに書き込むようにする。
	cat file2 実行
		file2 から読み込み、パイプへ書き込み。書き込み口は使い終わったら閉じる。
	元の STDOUT / STDIN に戻す
	pre_in_pipe_fd に in_pipe_fd を置き換える
		次のコマンドにとっては“前のパイプの読み取り口”になる。
	pre_in_pipe_fd を STDIN に差し替え
		次の cat コマンドはパイプからデータ（file2 の中身）を読み取るようになる。
	次の cat 実行
		cat はパイプのデータ（つまり file2 の内容）を受け取って標準出力に出す。

fd_num について (Heredoc)
「<< か < の最後が Heredoc である場合のみ fd_num に値をセットする形にしたので、
fd_num が -1 の時は基本無視してもらって大丈夫。」
Heredoc（<<）を扱うとき、しばしば一時ファイルやパイプの番号を保持するために
fd_numを使う。
fd_num = -1 なら「Heredocではない」という判定が可能。
何か特別な入出力（Heredoc用のFD）を持っているときだけ fd_num に有効な値が入っている設計。
*/
