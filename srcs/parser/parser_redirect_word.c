/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_word.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 04:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/12 19:49:55 by ssoeno           ###   ########.fr       */
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
			node->left->redirects[j++] = ft_strdup((*t_l)->word);
			*t_l = (*t_l)->next;
			if (!compare_tk(ND_CMD, t_l))
				d_throw_error("count_nodes_for_redirect", "invalid syntax");
			node->left->redirects[j++] = ft_strdup((*t_l)->word);
		}
		else if (compare_tk(ND_CMD, t_l))
			node->cmds[i++] = ft_strdup((*t_l)->word);
		*t_l = (*t_l)->next;
	}
	node->left->redirects[j] = NULL;
	node->cmds[i] = NULL;
	return (node);
}

t_node	*parse_redirects(t_token **token_list)
{
	t_node	*node;

	node = NULL;
	if (!compare_tk(ND_REDIRECTS, token_list))
		return (NULL);
	node = create_node(ND_REDIRECTS);
	node->redirects = parse_redirect_arry(token_list);
	return (node);
}

char	*parse_single_redirect(t_token **token_list)
{
	char	*word;

	if (!*token_list)
		d_throw_error("parse_single_redirect", "token_list is NULL");
	if ((*token_list)->kind == ND_EOF)
		d_throw_error("parse_single_redirect", "NULL token kind");
	word = ft_strdup((*token_list)->word);
	if (!word)
		d_throw_error("parse_single_redirect", "strdup_error");
	*token_list = (*token_list)->next;
	return (word);
}

void	handle_redirect_error(char **ret, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		free(ret[i]);
		i++;
	}
	free(ret);
	d_throw_error("handle_redirect_array", "redirect syntax error");
}
/*
free_wordlist(&ret[count - 1]) is wrong.
When iput is "> > file", ret[1] is not initialized in the parse_single_redirect
so free_wordlist(&ret[count - 1]) causes a segmentation fault.
TODO:need to change handle_redirect_error to check syntax error
*/

char	**parse_redirect_arry(t_token **token_list)
{
	char	**ret;
	int		i;
	size_t	rd_cnt;

	ret = NULL;
	i = 0;
	rd_cnt = count_nodes_for_rd(token_list);
	if (!rd_cnt)
		return (NULL);
	ret = xmalloc((rd_cnt + 1) * sizeof(char *));
	while (*token_list && compare_tk(ND_REDIRECTS, token_list))
	{
		ret[i++] = parse_single_redirect(token_list);
		if (!*token_list || (*token_list && !compare_tk(ND_CMD, token_list)))
			handle_redirect_error(ret, i);
		ret[i++] = parse_single_redirect(token_list);
	}
	ret[i] = NULL;
	return (ret);
}
