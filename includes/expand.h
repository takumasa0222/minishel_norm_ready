/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:27:18 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/02/01 09:57:05 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H
# define DOLLAR '$'
# define ASTERISK '*'
# define UNDER_SCORE '_'
# define QUESTION_MARK "?"
# include "./parser.h"
# include "./lexer.h"
# include "./map.h"
# include "./minishell.h"

void	expand_handler(t_node *node, t_context *ctx);
void	expand_redirect_handler(t_node *node, t_context *ctx);
void	expand_variable_handler(t_node *node, t_context *ctx);
char	*expand_variable(char *str, t_context *ctx);
int		expand_heredoc_var(t_node *node, t_context *ctx);
size_t	retrieve_val_in_sq(char **ret, char *str, size_t i);
size_t	skip_s_quote_block(char *str, size_t i);
bool	is_dollar_symbol(char c);
size_t	retrieve_val_in_dq(char **ret, char *str, size_t i, t_context *ctx);
char	*append_substring(char **ret, char *str, size_t i, size_t len);
size_t	retrieve_var(char **ret, char *str, size_t i, t_context *ctx);
size_t	retrieve_var_name_len(char *str, size_t i, bool is_heredoc);
size_t	retrieve_normal_val(char **ret, char *str, size_t i, bool is_heredoc);
void	remove_quote_handler(t_node *node);
char	*remove_quotes(char *str);
int		move_to_next_quotation_expnd(char *input, int i);
char	*expand_var_in_heredoc(char *str, t_context *ctx);
size_t	retrieve_var_in_heredoc(char **ret, char *s, size_t i, t_context *ctx);
void	expand_var_rd_handler(t_node *node, t_context *ctx);
void	expand_aster_rd_handler(t_node *node);
void	remove_quote_rd_handler(t_node *node);
size_t	recreate_redirect_list(t_node *node, char **file_arry, size_t i);
#endif