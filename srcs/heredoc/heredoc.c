/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:14:42 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/19 15:18:54 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"
#include <unistd.h>
#include "../includes/heredoc.h"
#include "../includes/signals.h"

static int	terminate_heredoc(int *pipe_fds);
int			read_heredoc_line(char *eof, int pipe_fd, t_context *ctx);

// int	input_heredoc_content(char *eof, t_context *ctx)
// {
// 	char			*line;
// 	int				pipe_fds[2];

// 	line = NULL;
// 	if (isatty(STDIN_FILENO))
// 		rl_event_hook = heredoc_sigint_event_hook;
// 	if (pipe(pipe_fds) == -1)
// 		d_throw_error("input_heredoc_content", "pipe failed");
// 	while (1)
// 	{
// 		free(line);
// 		line = NULL;
// 		line = readline("> ");
// 		if (g_sig == SIGINT)
// 		{
// 			ctx->heredoc_interrupted = true;
// 			return (terminate_heredoc(line, pipe_fds));
// 		}
// 		if (line == NULL || \
// 		(ft_strlen(line) == ft_strlen(eof) && !ft_strcmp(eof, line)))
// 			break ;
// 		else
// 		{
// 			write(pipe_fds[1], line, ft_strlen(line));
// 			write(pipe_fds[1], "\n", ft_strlen("\n"));
// 		}
// 	}
// 	if (isatty(STDIN_FILENO))
// 		rl_event_hook = sigint_event_hook;
// 	free(line);
// 	close(pipe_fds[1]);
// 	return (pipe_fds[0]);
// }
/*
by returning pipe_fds[0], we can redirect STDIN from this pipe
so that the command reads these lines as input.
*/

int	input_heredoc_content(char *eof, t_context *ctx)
{
	int	pipe_fds[2];
	int	ret;

	if (isatty(STDIN_FILENO))
		rl_event_hook = heredoc_sigint_event_hook;
	if (pipe(pipe_fds) == -1)
		d_throw_error("input_heredoc_content", "pipe failed");
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

static int terminate_heredoc(int *pipe_fds)
{
	close(pipe_fds[1]);
	if (isatty(STDIN_FILENO))
		rl_event_hook = sigint_event_hook;
	return (-2);
}

void	heredoc_handler(t_node *node, t_context *ctx)
{
	if (node == NULL)
		return ;
	heredoc_handler(node->left, ctx);
	heredoc_handler(node->right, ctx);
	if (node->kind == ND_REDIRECTS)
		call_heredoc(node, ctx);
}

void	call_heredoc(t_node *node, t_context *ctx)
{
	size_t	i;
	size_t	j;
	int		*temp_fd_arry;

	i = 0;
	j = 0;
	if (!node || !node->redirects)
		d_throw_error("call_heredoc", "node or redirects is null");
	temp_fd_arry = \
	xmalloc((get_char_arry_size(node->redirects) / 2 + 1) * sizeof (int));
	while (node->redirects[i])
	{
		if (!ft_strcmp(node->redirects[i], "<<") && node->redirects[i + 1])
		{
			temp_fd_arry[j++] = input_heredoc_content(node->redirects[i + 1], ctx);
			i++;
		}
		i++;
	}
	if (j > 0 && is_heredoc_end(node->redirects))
		node->fd_num = temp_fd_arry[--j];
	close_unused_fds(temp_fd_arry, j);
	free(temp_fd_arry);
}
/*
temp_fd_array: store multiple feredoc fds
Everytime it sees a "<<" it calls input_heredoc_content()
with the next string(node->redirects[i+1]) as the end marker.
The returned FD from input_heredoc_content() is stored in temp_fd_array
Assign node->fd_num
	- if j > 0 (actually created at least one heredoc FD)
	- is_heredoc_end(node->redirects) is true
	- node->fd_num is set to temp_fd_arry[--j] (presumably means 
	　"use the last heredoc FD as the input for this node")
*/

bool	is_heredoc_end(char **redirects)
{
	size_t	i;
	bool	ret;

	i = 0;
	ret = false;
	if (!redirects || !redirects[i])
		d_throw_error("retrieve_last_input", "redirects is null");
	while (redirects[i])
	{
		if (!ft_strcmp(redirects[i], "<"))
			ret = false;
		else if (!ft_strcmp(redirects[i], "<<"))
			ret = true;
		i++;
	}
	return (ret);
}

void	close_unused_fds(int *arry, size_t end)
{
	size_t	i;

	i = 0;
	if (!end || !arry || end < 1)
		return ;
	end = end - 1;
	while (i < end && arry[i])
	{
		close(arry[i]);
		i++;
	}
}
/*
This function closes all the file descriptors except for the final one.
end: the number of active FDs, 
and then we do end = end - 1 to exclude the one we’re keeping.
The loop closes everything from arry[0] up to arry[end - 1]. 
This effectively leaves the last FD open if you only need the final one.
Key Point: If the code has multiple <<, 
it’s presumably discarding all but the last one. 
The final FD is used by node->fd_num.
*/