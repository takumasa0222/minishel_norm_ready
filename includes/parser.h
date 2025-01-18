/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 02:46:35 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/19 00:44:45 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"
# include "minishell.h"

typedef struct s_node	t_node;
// AST node type
struct s_node {
	t_node_kind	kind;
	t_node		*left;
	t_node		*right;
	char		**cmds;
	char		*op_val;
	int			fd_num;
	char		**redirects;
};

//parser.c
t_node	*parse_cmd_handler(t_token **token_list, t_syntax_error *err);
t_node	*parse_cmd(t_token **token_list, t_syntax_error *syntx_err);
t_node	*parse_cmd_type(t_token **token_list, t_syntax_error *syntx_err);
t_node	*simple_cmd(t_token **token_list, t_syntax_error *syntx_err);
t_node	*parse_subshell(t_token **token_list, t_syntax_error *syntx_err);
char	**parse_words(t_token **token_list);

//parser_cmd_tail.c
t_node	*parse_cmd_tail(t_node *left, t_token **tk_list, t_syntax_error *err);
t_node	*handle_logical_op(t_node *left, t_token **tk_list, t_syntax_error *e);
t_node	*handle_redirects(t_node *left, t_token **tk_list, t_syntax_error *e);
//parser_node_operator.c
t_node	*create_logi_node(t_node *left, t_token **t_l, t_syntax_error *err);
t_node	*create_node(t_node_kind token_kind);
t_node	*create_seq_pipe_node(t_node **prnt, t_token **tl, t_syntax_error *e);

//parser_token_checker.c
bool	match_token(t_node_kind expected, t_token **cur_token);
bool	compare_tk(t_node_kind expected, t_token **cur_token);
bool	is_cmd_parsable_tk(t_token **cur_token);
//parser_redirect_word.c
t_node	*parse_cmd_rd_node(t_token **t_l, t_node *node, size_t cmd, size_t rd);
t_node	*parse_redirects(t_token **token_list, t_syntax_error *syntx_err);
char	*parse_single_redirect(t_token **token_list);
char	**parse_redirect_arry(t_token **token_list, t_syntax_error *syntx_err);
//parser_node_counter.c
size_t	count_nodes(t_token **token_list, t_node_kind kind);
size_t	count_nodes_for_rd(t_token **token_list, t_syntax_error *err);
size_t	cnt_cmd_rd(t_token **tl, size_t *wc, size_t *rc, t_syntax_error *err);
//parser_error_handler.c
void	parser_syntax_err(char ***ar, t_syntax_error *e, t_node **n, char *msg);
void	log_syntax_err(char ***ar, t_syntax_error *e, t_node **n);
#endif