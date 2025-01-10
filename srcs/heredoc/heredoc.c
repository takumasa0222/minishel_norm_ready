/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:14:42 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/10 22:20:16 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"
#include <unistd.h>
#include "../includes/heredoc.h"

int	input_heredoc_content(char *eof)
{
	char	*line;
	int		pipe_fds[2];

	line = NULL;
	if (pipe(pipe_fds))
		d_throw_error("input_heredoc_content", "pipe failed");
	while (1)
	{
		free(line);
		line = NULL;
		line = readline("> ");
		if (line == NULL || \
		(ft_strlen(line) == ft_strlen(eof) && !ft_strcmp(eof, line)))
			break ;
		else
		{
			write(pipe_fds[1], line, ft_strlen(line));
			write(pipe_fds[1], "\n", ft_strlen("\n"));
		}
	}
	free(line);
	close(pipe_fds[1]);
	return (pipe_fds[0]);
}

void	heredoc_handler(t_node *node)
{
	if (node == NULL)
		return ;
	heredoc_handler(node->left);
	heredoc_handler(node->right);
	if (node->kind == ND_REDIRECTS)
		call_heredoc(node);
}

void	call_heredoc(t_node *node)
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
			temp_fd_arry[j++] = input_heredoc_content(node->redirects[i + 1]);
			i++;
		}
		i++;
	}
	if (j > 0 && is_heredoc_end(node->redirects))
		node->fd_num = temp_fd_arry[--j];
	free(temp_fd_arry);
}

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
