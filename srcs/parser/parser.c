/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:51:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/15 18:59:43 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

/*
this parser will create AST based on below eBNF.
    start: command
	command: cmd_type command_tail
	command_tail: "|" cmd_type command_tail 
            | ("&&" | "||") cmd_type command_tail
            | redirection
			|
	cmd_type: simple_command | subshell
	subshell: "(" command ")" command_tail*
	simple_command: redirection* wordlist redirection* | redirection+
    wordlist: WORD | wordlist WORD
    redirection: ">" filename
               | "<" filename
               | ">>" filename
               | "<<" filename
               | "<>" filename
               | NUMBER ">" filename
               | NUMBER "<" filename
               | NUMBER ">>" filename
               | NUMBER "<<" filename
               | NUMBER "<>" filename
    filename: WORD
    WORD: /[a-zA-Z0-9_"]+/
    NUMBER: /[0-9]+/
				
*/

/*
今あまり明確になっていないところ
・どんなときならノードを作るのか -> オペレーターもしくはコマンドのときのみ
・親ノードを追加する場合や子ノードを追加するときの切り分け方　-> 実際にほしい木を意識してみるとわかりやすい
・現在の問題点 ((ls)) のような場合にパーサーエラーが発生しない
・match_token を行う場合と単純に判定を行う場合で分ける必要がありそう。基本的には、match は実際のトークン作成時に行うのがベター
・match_token を行う際に実際のトークンも勧めていく形にする
修正ポイント　11/17
・match_token / compare_tk の箇所は、t_token_kind に合わせる。一方で、t_token_kind の種類を増やす
*/

t_node	*parse_cmd(t_token **token_list)
{
	t_node	*node;

	node = NULL;
	node = parse_cmd_type(token_list);
	if (!node)
		d_throw_error("parse_cmd", "node is empty");
	return (parse_cmd_tail(node, token_list));
}

t_node	*parse_cmd_type(t_token **token_list)
{
	t_node	*node;

	if (compare_tk(ND_L_PARE, token_list))
	{
		node = parse_subshell(token_list);
		return (node);
	}	
	else
		return (simple_cmd(token_list));
}

t_node	*simple_cmd(t_token **token_list)
{
	t_node	*node;
	t_node	*tmp;

	node = create_node(ND_CMD);
	if (compare_tk(ND_REDIRECTS, token_list))
		node->left = parse_redirects(token_list);
	node->cmds = parse_words(token_list);
	if (!node->cmds && !node->left)
	{
		free(node);
		return (NULL);
	}
	else if (!node->cmds && node->left)
	{
		tmp = node->left;
		free(node);
		return (tmp);
	}
	else if (compare_tk(ND_REDIRECTS, token_list))
		node->right = parse_redirects(token_list);
	return (node);
}

t_node	*parse_subshell(t_token **token_list)
{
	t_node	*node;

	if (!match_token(ND_L_PARE, token_list))
		d_throw_error("parser_subshell", "syntax_error");
	node = create_node(ND_SUB_SHELL);
	node->left = parse_cmd(token_list);
	if (!match_token(ND_R_PARE, token_list))
		d_throw_error("parser_subshell", "syntax_error");
	return (node);
}

t_node	*parse_cmd_tail(t_node *left, t_token **tk_list)
{
	t_node	*node;

	node = NULL;
	if (match_token(ND_PIPE, tk_list))
	{
		node = create_node(ND_PIPE);
		node->left = left;
		node->right = parse_cmd_type(tk_list);
		create_sequential_pipe_node(node, tk_list);
		return (parse_cmd_tail(node, tk_list));
	}
	else if (compare_tk(ND_AND_OP, tk_list) || compare_tk(ND_OR_OP, tk_list))
	{
		node = create_logi_node(left, tk_list);
		if (compare_tk(ND_PIPE, tk_list))
		{
			node->right = parse_cmd_tail(node->right, tk_list);
			return (node);
		}
		else
			return (parse_cmd_tail(node, tk_list));
	}
	else if (compare_tk(ND_REDIRECTS, tk_list))
	{
		t_node *redirect_node = parse_redirects(tk_list);
		if (redirect_node == NULL)
			return (left);
		if (left)
		{
			left->right = redirect_node;
			return (left);
		}
		else 
		{
			return (redirect_node);
		}
		// return (parse_redirects(tk_list));
	}
	return (left);
}
