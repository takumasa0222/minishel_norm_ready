/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:49:42 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/13 21:49:46 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H
# include "./minishell.h"
# include "./parser.h"

// void	set_redirect_fds(t_node *node, t_context *ctx);
void	set_redirect_fds(t_node *node);
void    restore_std_fds(t_context *ctx);

// redirect_utils.c
void	redirect_in(char *filename);
void	redirect_out(char *filename);
void	redirect_append(char *filename);
void	redirect_here_doc(t_node *node);

#endif
