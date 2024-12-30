/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:34:45 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/30 21:20:06 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "minishell.h"
# include "parser.h"

// error.c
void	d_throw_error(char *func_name, char *error_msg);
void	builtin_error(char *func, char *name, char *err);

// free_functions.c
void	free_node(t_node *node);
void	free_wordlist(char **wordlist);
void	free_ast(t_node **node);
void	free_token_list(t_token *token_list);

// xmalloc.c
void	*xmalloc(size_t size);
#endif