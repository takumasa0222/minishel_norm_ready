/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:14:42 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/08 01:31:24 by tamatsuu         ###   ########.fr       */
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
	temp_fd_arry[j] = 0;
	node->fd_num = temp_fd_arry[--j];
	close_fd_arry(&temp_fd_arry, j);
	free(temp_fd_arry);
}

void	close_fd_arry(int **arry, size_t end)
{
	size_t	i;

	if (!arry || !*arry || !end)
		return ;
	i = 0;
	while (i < end)
	{
		close(*arry[i]);
		i++;
	}
}

	// if (j)
	// {
	// 	int f = 0;
	// 	while (j--)
	// 	{
	// 		char test[128];
	// 		int readret = read(fds[f], test, sizeof(test) - 1);
	// 		printf("readret: %d\n",readret);
	// 		printf("heredoc_test: %s\n",test);
	// 		f++;
	// 	}
	// }

// close_heredoc()
// {
	
// }
