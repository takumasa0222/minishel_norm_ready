/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_tail.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 22:25:13 by ssoeno            #+#    #+#             */
/*   Updated: 2024/12/22 18:38:47 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

// ND_PIPE
static t_node	*handle_pipe(t_node *left, t_token **tk_list)
{
	t_node	*node;

	node = create_node(ND_PIPE);
	if (!node)
	{
		free_node(left);
		d_throw_error("handle_pipe", "failed to create pipe node");
		return (NULL);
	}
	node->left = left;
	node->right = parse_cmd_type(tk_list);
	if (!node->right)
	{
		free_node(node);
		return (NULL);
	}
	create_sequential_pipe_node(node, tk_list);
	return (node);
}

// ND_AND_OP oe ND_OR_OP
static t_node	*handle_logical_op(t_node *left, t_token **tk_list)
{
	t_node	*node;

	node = create_logi_node(left, tk_list);
	if (!node)
	{
		free_node(left);
		d_throw_error("handle_logical_op", "failed to create logi node");
		return (NULL);
	}
	if (compare_tk(ND_PIPE, tk_list))
	{
		node->right = parse_cmd_tail(node->right, tk_list);
		if (!node->right)
		{
			free_node(node);
			return (NULL);
		}
	}
	return (node);
}

// ND_REDIRECTS
static t_node	*handle_redirects(t_node *left, t_token **tk_list)
{
	t_node	*redirect_node;

	redirect_node = parse_redirects(tk_list);
	if (!redirect_node)
		return (left);
	if (left)
		left->right = redirect_node;
	else
		left = redirect_node;
	return (left);
}

t_node	*parse_cmd_tail(t_node *left, t_token **tk_list)
{
	while (true)
	{
		if (match_token(ND_PIPE, tk_list))
			left = handle_pipe(left, tk_list);
		else if (compare_tk(ND_AND_OP, tk_list)
			|| compare_tk(ND_OR_OP, tk_list))
			left = handle_logical_op(left, tk_list);
		else if (compare_tk(ND_REDIRECTS, tk_list))
			left = handle_redirects(left, tk_list);
		else
			break ;
		if (!left)
			return (NULL);
	}
	return (left);
}
