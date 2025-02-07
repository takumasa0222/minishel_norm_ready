/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_tail.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 22:25:13 by ssoeno            #+#    #+#             */
/*   Updated: 2025/02/04 03:05:58 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

// ND_PIPE
static t_node	*handle_pipe(t_node *left, t_token **t_l, t_syntax_error *err)
{
	t_node	*node;

	if (!left)
		return (NULL);
	node = create_node(ND_PIPE);
	node->left = left;
	if (!is_cmd_parsable_tk(t_l))
		parser_syntax_err(NULL, err, &node, ERR_MSG_PIPE);
	if (!node)
		return (NULL);
	node->right = parse_cmd_type(t_l, err);
	if (!node->right)
	{
		parser_syntax_err(NULL, err, &node, ERR_MSG_PIPE);
		return (NULL);
	}
	if (!create_seq_pipe_node(&node, t_l, err))
		free_ast(&node);
	return (node);
}

// ND_AND_OP or ND_OR_OP
t_node	*handle_logical_op(t_node *left, t_token **tk_list, t_syntax_error *e)
{
	t_node	*node;

	node = create_logi_node(left, tk_list, e);
	if (!node)
		return (NULL);
	if (compare_tk(ND_PIPE, tk_list))
	{
		node->right = parse_cmd_tail(node->right, tk_list, e);
		if (!node->right)
		{
			parser_syntax_err(NULL, e, &node, ERR_MSG_PIPE);
			return (NULL);
		}
	}
	return (node);
}

// ND_REDIRECTS
t_node	*handle_redirects(t_node *left, t_token **tk_list, t_syntax_error *e)
{
	t_node	*redirect_node;

	redirect_node = parse_redirects(tk_list, e);
	if (!redirect_node)
		return (left);
	if (left)
		left->right = redirect_node;
	else
		left = redirect_node;
	return (left);
}

t_node	*parse_cmd_tail(t_node *left, t_token **tk_list, t_syntax_error *err)
{
	while (true)
	{
		if (!left)
			return (NULL);
		if (match_token(ND_PIPE, tk_list))
			left = handle_pipe(left, tk_list, err);
		else if (compare_tk(ND_AND_OP, tk_list)
			|| compare_tk(ND_OR_OP, tk_list))
			left = handle_logical_op(left, tk_list, err);
		else if (compare_tk(ND_REDIRECTS, tk_list))
			left = handle_redirects(left, tk_list, err);
		else
			break ;
	}
	return (left);
}
