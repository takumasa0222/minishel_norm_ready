/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:14:42 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/19 16:50:08 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"
#include <unistd.h>
#include "../includes/heredoc.h"
#include "../includes/signals.h"

static int	terminate_heredoc(int *pipe_fds);
static int	read_heredoc_line(char *eof, int pipe_fd, t_context *ctx);

int	read_heredoc(char *eof, t_context *ctx)
{
	int	pipe_fds[2];
	int	ret;

	if (isatty(STDIN_FILENO))
		rl_event_hook = heredoc_sigint_event_hook;
	if (pipe(pipe_fds) == -1)
	{
		if (isatty(STDIN_FILENO))
			rl_event_hook = sigint_event_hook;
		d_throw_error("read_heredoc", "pipe failed");
	}
	ret = read_heredoc_line(eof, pipe_fds[1], ctx);
	if (isatty(STDIN_FILENO))
		rl_event_hook = sigint_event_hook;
	close(pipe_fds[1]);
	if (ret < 0)
		return (terminate_heredoc(pipe_fds));
	return (pipe_fds[0]);
}

int	read_heredoc_line(char *eof, int pipe_fd, t_context *ctx)
{
	char	*line;

	line = NULL;
	while (1)
	{
		free(line);
		line = NULL;
		line = readline("> ");
		if (g_sig == SIGINT)
		{
			ctx->heredoc_interrupted = true;
			free (line);
			return (-2);
		}
		if (line == NULL || \
		(ft_strlen(line) == ft_strlen(eof) && !ft_strcmp(eof, line)))
			break ;
		write(pipe_fd, line, ft_strlen(line));
		write(pipe_fd, "\n", ft_strlen("\n"));
	}
	free(line);
	return (EXIT_SUCCESS);
}

static int	terminate_heredoc(int *pipe_fds)
{
	close(pipe_fds[1]);
	if (isatty(STDIN_FILENO))
		rl_event_hook = sigint_event_hook;
	return (-2);
}
