/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:51:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/10 22:21:29 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"

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
・どんなときならノードを作るのか
・親ノードを追加する場合や子ノードを追加するときの切り分け方
・

*/

t_node	*entry_parser(t_token *token_list)
{
	t_node	*ret;

	ret = NULL;
	return (parse_cmd(&token_list));
}

t_node	*parse_cmd(t_token **token_list)
{
	t_node	*node;

	node = parse_cmd_type();
	if (!node)
		d_throw_error();
	return (parse_cmd_tail(node));
}

t_node	*parse_cmd_type(t_node *node, t_token **token_list)
{
	t_node	*ret;

	// should not proceed token since in the parse_subshell match_token will be used.
	if (is_left_pare(ND_L_PARE, token_list))
		return (parse_subshell(token_list));
	else
		return (simple_cmd(token_list));
}

t_node	*simple_cmd(t_token **token_list)
{
	t_node	*node;

	node = create_node(ND_CMD);
	//add redirect node and the node can contain multi redirect cmds
	if (match_token(ND_REDIRECT, token_list))
		node->left = parse_redirect(token_list);
	//IF NEXT TOKEN IS WORD THEN 
	node->cmds = word_list();
	if (!node->cmds)
		//remove this node since there is no words.
		//evaluation should be done by simple_command1
		// token_list should be reset before this function exec. 
	if (match_token(ND_REDIRECT, token_list))
	{
		node->right = parse_redirect(token_list);
	}
	return (node);
}

t_node	*parse_redirect(t_token **token_list)
{

}

t_node	*parse_subshell(t_node *node, t_token **token_list)
{
	t_node	*node;
	
	if(!match_token(ND_L_PARE))
		d_throw_error("parser_subshell","syntax_error");
	node = create_node(ND_SUB_SHELL);
	node->left = parse_cmd();
	if(!match_token(ND_R_PARE))
		d_throw_error("parser_subshell","syntax_error");
	return (parse_cmd_tail(node, token_list));
}

t_node	*parse_cmd_tail(t_node *left, t_token **token_list)
{
	t_node	*node;

	if (match_token(ND_PIPE)) 
	{
		node = create_node(ND_PIPE);
		node->left = left;
        node->right = parse_cmd_type();
		return (parse_cmd_tail(node, token_list));
	}
	else if (match_token(ND_AND_OP) || match_token(ND_OR_OP))
	{
		if (match_token (ND_AND_OP))
			node = create_node(ND_AND_OP);
		else
			node = create_node(ND_OR_OP);
		node->left = left;
		node->right = parse_cmd_type();
		return (parse_cmd_tail(node, token_list));
	}
	else if (is_redirection(current_token))
		return (parse_redirection(left));
    return (left);
}

bool	match_token(t_node_kind expected, t_token **cur_token)
{
	if ((*cur_token)->kind == expected)
	{
		*cur_token = (*cur_token)->next;
		return (true);
	}
	return (false);
}

t_node	*create_node(t_node_kind token_kind)
{
	t_node	*new_node;

	new_node = malloc(1 * sizeof(t_node));
	if (!new_node)
		d_throw_error("create_node", "malloc failed");
	new_node->kind = token_kind;
	return (new_node);
}

match_subshell()
{
	
}
