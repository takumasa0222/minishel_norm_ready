/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/18 12:47:15 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"
#include "../includes/redirect.h"
#include "../includes/execute.h"

int	apply_redirects(t_node *node);

// void	set_redirect_fds(t_node *node, t_context *ctx)
void	set_redirect_fds(t_node *node)
{
	// backup_std_fds(ctx);
	apply_redirects(node->left);
}

int	apply_redirects(t_node *node)
{
	int		i;
	char	*op;
	char	*filename;

	i = 0;
	while (node->redirects[i])
	{
		op = node->redirects[i];
		filename = node->redirects[i + 1];
		if (!filename)
			d_throw_error("apply_redirect_fds", "filename is NULL");
		if (ft_strcmp(op, ">") == 0)
			redirect_out(filename);
		else if (ft_strcmp(op, ">>") == 0)
			redirect_append(filename);
		else if (ft_strcmp(op, "<") == 0)
			redirect_in(filename);
		else if (ft_strcmp(op, "<<") == 0)
			redirect_here_doc(node);
		else
			d_throw_error("apply_redirect_fds", "unexpected operator");
		i += 2;
	}
	return (EXIT_SUCCESS);
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