/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:27:18 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/18 20:41:17 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H
# define DOLLAR '$'
# define ASTERISK '*'
# define UNDER_SCORE '_'
# include "./parser.h"
# include "./lexer.h"
# include "./builtin.h"

void	expand_handler(t_node *node, char **envp);
void	expand_variable_handler(t_node *node, char **envp);
char	*expand_variable(char *str, char **envp);
size_t	retrieve_val_in_sq(char **ret, char *str, size_t i);
size_t	skip_s_quote(char *str, size_t i);
bool	is_dollar_symbol(char c);
size_t	retrieve_val_in_dq(char **ret, char *str, size_t i, char **envp);
char	*str_concat_helper(char **ret, char *str, size_t i, size_t len);
size_t	retrieve_var(char **ret, char *str, size_t i, char **envp);
size_t	retrieve_var_name_len(char *str, size_t i);
t_map	*init_envmap(char **envp);
size_t	retrieve_normal_val(char **ret, char *str, size_t i);

#endif
