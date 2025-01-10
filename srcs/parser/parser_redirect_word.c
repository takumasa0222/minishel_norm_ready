/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_word.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 04:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/10 21:09:59 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

// combine count_word_node and count_rd_node
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
		d_throw_error("count_nodes_for_redirect", "unexpected result");
	while (compare_tk(ND_REDIRECTS, &temp))
	{
		temp = temp->next;
		if (!compare_tk(ND_CMD, &temp))
			d_throw_error("count_nodes_for_redirect", "invalid syntax");
		count = count + 2;
		temp = temp->next;
	}
	if (!count || count % 2)
		d_throw_error("count_nodes_for_redirect", "unexpected result");
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
				d_throw_error("count_nodes_for_redirect", "invalid syntax");
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

// t_node	*parse_redirects_rnode(t_token **token_list, t_node *cmd_node)
// {
// 	t_node	*ret;
// 	size_t	rd_cnt;
// 	size_t	total_cnt;

// 	ret = NULL;
// 	if (!compare_tk(ND_REDIRECTS, token_list))
// 		return (NULL);
// 	total_cnt = 0;
// 	rd_cnt = count_nodes_rnode_redirect(token_list, &total_cnt);
// 	if (rd_cnt == total_cnt)
// 		return (parse_redirects(token_list));
// 	ret = create_node(ND_REDIRECTS);
// 	ret->redirects = parse_redirect_rnode_arry(token_list, word_lst);
// }

// char	**parse_redirect_rnode_arry(t_token **tk_list, char ***word_lst)
// {
// 	char	**ret;
// 	int		i;
// 	int		j;
// 	size_t	rd_cnt;

// 	i = 0;
// 	j = 0;
// 	rd_cnt = count_nodes_rnode_redirect(tk_list, NULL);
// 	ret = xmalloc((rd_cnt + 1) * sizeof(char *));
// 	while (*tk_list && \
// 	(compare_tk(ND_REDIRECTS, tk_list) || compare_tk(ND_CMD, tk_list)))
// 	{
// 		if (compare_tk(ND_REDIRECTS, tk_list))
// 		{
// 			ret[i++] = parse_single_redirect(tk_list);
// 			if (!*tk_list || (*tk_list && !compare_tk(ND_CMD, tk_list)))
// 				handle_redirect_error(ret, i);
// 			ret[i++] = parse_single_redirect(tk_list);
// 		}
// 		else
// 			*word_lst[j++] = parse_single_redirect(tk_list);
// 	}	
// 	return (ret[i] = NULL,*word_lst[j] = NULL, ret);
// }
