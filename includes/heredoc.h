/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 13:31:55 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/28 22:50:55 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H
# include "./minishell.h"
# include "./parser.h"
# define SIGINT_DURING_HEREDOC -2

// heredoc_read.c
int		read_heredoc(char *eof, t_context *ctx);

// heredoc.c
bool	heredoc_handler(t_node *node, t_context *ctx, bool *is_sigint);
void	call_heredoc(t_node *node, t_context *ctx);
bool	is_heredoc_end(char **redirects);
void	close_unused_fds(int *arry, size_t end);

#endif