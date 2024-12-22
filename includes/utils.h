/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:34:45 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/22 19:08:02 by ssoeno           ###   ########.fr       */
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
void	free_str_array(char **array);

// xmalloc.c
void	*xmalloc(size_t size);

#endif