/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 13:31:55 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/19 00:55:33 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H
# include "./minishell.h"
# include "./parser.h"

int		input_heredoc_content(char *eof, t_context *ctx);
void	heredoc_handler(t_node *node, t_context *ctx);
void	call_heredoc(t_node *node, t_context *ctx);
bool	is_heredoc_end(char **redirects);
void	close_unused_fds(int *arry, size_t end);

#endif