/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_node_operator.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 04:40:41 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/14 02:34:43 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

t_node	*create_node(t_node_kind token_kind)
{
	t_node	*new_node;

	new_node = xmalloc(1 * sizeof(t_node));
	new_node->kind = token_kind;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->cmds = NULL;
	new_node->redirects = NULL;
	new_node->op_val = NULL;
	new_node->fd_num = -1;
	return (new_node);
}

t_node	*create_logi_node(t_node *left, t_token **token_list)
{
	t_node	*node;

	node = NULL;
	if (match_token(ND_AND_OP, token_list))
		node = create_node(ND_AND_OP);
	else if (match_token(ND_OR_OP, token_list))
		node = create_node(ND_OR_OP);
	else
	{
		d_throw_error("create_logi_node", "Expected logical operator");//unexpected error
		return (NULL);
	}
	node->left = left;
	node->right = parse_cmd_type(token_list);//syntax error could be caused if node->right is null.
	return (node);
}

t_node	*create_pipe_node(t_node *left, t_token **token_list)
{
	t_node	*node;

	node = NULL;
	node = create_node(ND_PIPE);
	node->left = left;
	node->right = parse_cmd_type(token_list);//syntax error could be caused if node->right is null.
	if (compare_tk(ND_PIPE, token_list))
		create_sequential_pipe_node(node, token_list);
	return (node);
}

t_node	*create_sequential_pipe_node(t_node *parent, t_token **token_list)
{
	t_node	*child;

	if (match_token(ND_PIPE, token_list))
	{
		child = create_node(ND_PIPE);
		child->left = parent->right;
		parent->right = child;
		child->right = parse_cmd_type(token_list);//syntax error could be caused if node->right is null.
		return (create_sequential_pipe_node(child, token_list));
	}
	return (parent);
}
