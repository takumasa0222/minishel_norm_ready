/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:49:42 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/02/01 02:06:13 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H
# include "./minishell.h"
# include "./parser.h"

int		set_redirect_fds(t_node *node, t_context *ctx);
void	restore_std_fds(t_context *ctx);
int		apply_redirects(t_node *node, t_context *ctx);
int		redirect_ambiguous_error(t_context *ctx);

// redirect_utils.c
void	redirect_in(char *filename, t_context *ctx);
void	redirect_out(char *filename, t_context *ctx);
void	redirect_append(char *filename, t_context *ctx);
void	redirect_here_doc(t_node *node, t_context *ctx);

#endif
