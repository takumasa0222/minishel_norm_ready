/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_node_counter.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 02:59:36 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/18 16:02:20 by tamatsuu         ###   ########.fr       */
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

size_t	count_nodes_for_rd(t_token **token_list, t_syntax_error *err)
{
	size_t	count;
	t_token	*temp;

	count = 0;
	temp = *token_list;
	if (!temp)
		throw_unexpected_error("count_nodes_for_redirect", NULL);
	while (compare_tk(ND_REDIRECTS, &temp))
	{
		temp = temp->next;
		if (!compare_tk(ND_CMD, &temp))
		{
			parser_syntax_err(NULL, err, NULL, ERR_MSG_RD);
			return (0);
		}
		count = count + 2;
		temp = temp->next;
	}
	if (!count || count % 2)
		throw_unexpected_error("count_nodes_for_redirect", NULL);
	return (count);
}

size_t	cnt_cmd_rd(t_token **tl, size_t *c_c, size_t *r_c, t_syntax_error *err)
{
	size_t	redirect_node_cnt;
	size_t	command_node_cnt;
	t_token	*tmp;

	redirect_node_cnt = 0;
	command_node_cnt = 0;
	tmp = *tl;
	while (tmp && (compare_tk(ND_REDIRECTS, &tmp) || compare_tk(ND_CMD, &tmp)))
	{
		if (compare_tk(ND_REDIRECTS, &tmp))
		{
			tmp = tmp->next;
			if (!compare_tk(ND_CMD, &tmp))
				return (parser_syntax_err(NULL, err, NULL, ERR_MSG_RD), 0);
			redirect_node_cnt = redirect_node_cnt + 2;
		}
		else if (compare_tk(ND_CMD, &tmp))
			command_node_cnt++;
		tmp = tmp->next;
	}
	if (r_c)
		*r_c = redirect_node_cnt;
	if (c_c)
		*c_c = command_node_cnt;
	return (redirect_node_cnt + command_node_cnt);
}
