/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_node_counter.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 02:59:36 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/13 03:42:19 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

size_t	count_nodes(t_token **token_list, t_node_kind kind)
{
	size_t	count;
	t_token	*temp;

	count = 0;
	temp = *token_list;
	if (kind == ND_REDIRECTS)
	{
		while (compare_tk(kind, &temp) || compare_tk(ND_CMD, &temp))
		{
			count++;
			temp = temp->next;
		}
	}
	else
	{
		while (compare_tk(kind, &temp))
		{
			count++;
			temp = temp->next;
		}
	}
	return (count);
}

size_t	count_nodes_for_rd(t_token **token_list)
{
	size_t	count;
	t_token	*temp;

	count = 0;
	temp = *token_list;
	if (!temp)
		d_throw_error("count_nodes_for_redirect", "unexpected result");//unexpected error
	while (compare_tk(ND_REDIRECTS, &temp))
	{
		temp = temp->next;
		if (!compare_tk(ND_CMD, &temp))
			d_throw_error("count_nodes_for_redirect", "invalid syntax");//syntax error
		count = count + 2;
		temp = temp->next;
	}
	if (!count || count % 2)
		d_throw_error("count_nodes_for_redirect", "unexpected result");//unexpected error
	return (count);
}

size_t	count_nodes_cmd_rd(t_token **tk_list, size_t *cmd_cnt, size_t *rd_cnt)
{
	size_t	redirect_node_cnt;
	size_t	command_node_cnt;
	t_token	*temp;

	redirect_node_cnt = 0;
	command_node_cnt = 0;
	temp = *tk_list;
	while (compare_tk(ND_REDIRECTS, &temp) || compare_tk(ND_CMD, &temp))
	{
		if (compare_tk(ND_REDIRECTS, &temp))
		{
			temp = temp->next;
			if (!compare_tk(ND_CMD, &temp))
				d_throw_error("count_nodes_for_redirect", "invalid syntax");//syntax error file should be there
			redirect_node_cnt = redirect_node_cnt + 2;
		}
		else if (compare_tk(ND_CMD, &temp))
			command_node_cnt++;
		temp = temp->next;
	}
	if (rd_cnt)
		*rd_cnt = redirect_node_cnt;
	if (cmd_cnt)
		*cmd_cnt = command_node_cnt;
	return (redirect_node_cnt + command_node_cnt);
}
