/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_node_operator.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 04:40:41 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/18 22:41:36 by tamatsuu         ###   ########.fr       */
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

t_node	*create_logi_node(t_node *left, t_token **tk_list, t_syntax_error *err)
{
	t_node	*node;

	node = NULL;
	if (match_token(ND_AND_OP, tk_list))
		node = create_node(ND_AND_OP);
	else if (match_token(ND_OR_OP, tk_list))
		node = create_node(ND_OR_OP);
	else
	{
		throw_unexpected_error("create_logi_node", "Expected logical operator");
		return (NULL);
	}
	node->left = left;
	if (!is_cmd_parsable_tk(tk_list))
		log_syntax_err(NULL, err, &node);
	if (!node)
		return (NULL);
	node->right = parse_cmd_type(tk_list, err);
	if (!node->left || !node->right)
		log_syntax_err(NULL, err, &node);
	return (node);
}

t_node	*create_seq_pipe_node(t_node **parent, t_token **tl, t_syntax_error *e)
{
	t_node	*child;

	if (match_token(ND_PIPE, tl))
	{
		child = create_node(ND_PIPE);
		child->left = (*parent)->right;
		(*parent)->right = child;
		if (!is_cmd_parsable_tk(tl))
			parser_syntax_err(NULL, e, parent, ERR_MSG_PIPE);
		if (!*parent)
			return (NULL);
		child->right = parse_cmd_type(tl, e);
		if (!child->right || !child->left)
		{
			parser_syntax_err(NULL, e, parent, ERR_MSG_PIPE);
			return (NULL);
		}
		return (create_seq_pipe_node(&child, tl, e));
	}
	return (*parent);
}

t_node	*parse_subshell(t_token **token_list, t_syntax_error *syntx_err)
{
	t_node	*node;

	if (!match_token(ND_L_PARE, token_list))
		throw_unexpected_error("parser_subshell", NULL);
	node = create_node(ND_RND_BRACKET);
	if (compare_tk(ND_R_PARE, token_list))
	{
		parser_syntax_err(NULL, syntx_err, &node, ERR_MSG_SUBSHELL);
		return (NULL);
	}
	node->left = parse_cmd(token_list, syntx_err);
	if (!node->left || !match_token(ND_R_PARE, token_list))
	{
		free_ast(&node);
		syntx_err->is_error = true;
		syntx_err->err_msg = ERR_MSG_SUBSHELL;
	}
	return (node);
}
