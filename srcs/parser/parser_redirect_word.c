/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_word.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 04:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/19 17:50:30 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

t_node	*parse_cmd_rd_node(t_token **t_l, t_node *node, size_t cmd, size_t rd)
{
	size_t	i;
	size_t	j;

	node->left = create_node(ND_REDIRECTS);
	node->cmds = xmalloc((cmd + 1) * sizeof(char *));
	node->left->redirects = xmalloc((rd + 1) * sizeof(char *));
	i = 0;
	j = 0;
	while (compare_tk(ND_REDIRECTS, t_l) || compare_tk(ND_CMD, t_l))
	{
		if (compare_tk(ND_REDIRECTS, t_l))
		{
			node->left->redirects[j++] = x_strdup((*t_l)->word);
			*t_l = (*t_l)->next;
			if (!compare_tk(ND_CMD, t_l))
				throw_unexpected_error("count_nodes_for_redirect", NULL);
			node->left->redirects[j++] = x_strdup((*t_l)->word);
		}
		else if (compare_tk(ND_CMD, t_l))
			node->cmds[i++] = x_strdup((*t_l)->word);
		*t_l = (*t_l)->next;
	}
	node->left->redirects[j] = NULL;
	node->cmds[i] = NULL;
	return (node);
}

t_node	*parse_redirects(t_token **token_list, t_syntax_error *syntx_err)
{
	t_node	*node;

	node = NULL;
	if (!compare_tk(ND_REDIRECTS, token_list))
		throw_unexpected_error("parse_redirects", NULL);
	node = create_node(ND_REDIRECTS);
	node->redirects = parse_redirect_arry(token_list, syntx_err);
	return (node);
}

char	*parse_single_redirect(t_token **token_list)
{
	char	*word;

	if (!*token_list)
		throw_unexpected_error("parse_single_redirect", "token_list is NULL");
	if ((*token_list)->kind == ND_EOF)
		throw_unexpected_error("parse_single_redirect", "NULL token kind");
	word = x_strdup((*token_list)->word);
	*token_list = (*token_list)->next;
	return (word);
}

char	**parse_redirect_arry(t_token **token_list, t_syntax_error *syntx_err)
{
	char	**ret;
	size_t	i;
	size_t	rd_cnt;

	ret = NULL;
	i = 0;
	rd_cnt = count_nodes_for_rd(token_list, syntx_err);
	if (!rd_cnt)
		return (NULL);
	ret = xmalloc((rd_cnt + 1) * sizeof(char *));
	while (i < rd_cnt && *token_list && compare_tk(ND_REDIRECTS, token_list))
	{
		ret[i++] = parse_single_redirect(token_list);
		if (!*token_list || (*token_list && !compare_tk(ND_CMD, token_list)))
		{
			ret[i] = NULL;
			parser_syntax_err(&ret, syntx_err, NULL, ERR_MSG_RD);
			return (NULL);
		}
		ret[i++] = parse_single_redirect(token_list);
	}
	ret[i] = NULL;
	return (ret);
}
