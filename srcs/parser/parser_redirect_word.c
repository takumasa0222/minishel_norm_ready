/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_word.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 04:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/21 23:02:04 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

static void	*xmalloc(size_t size)
{
	void	*p;

	p = malloc(size);
	if (!p)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (p);
}

void free_str_array(char **array)
{
	if (!array)
		return;
	for (size_t i = 0; array[i]; i++)
		free(array[i]);
	free(array);
}

// combine count_word_node and count_rd_node
size_t count_nodes(t_token **token_list, t_node_kind kind)
{
	size_t	count;
	t_token	*temp;

	count = 0;
	temp = *token_list;
	while (temp && temp->kind == kind)
	{
		count++;
		temp = temp->next;
		if (kind == ND_REDIRECTS)
		{
			if (!temp || temp->kind != ND_CMD)
				break;
			count++;
			temp = temp->next;
		}
	}
	return (count);
}

// size_t	count_word_node(t_token	**token_list)
// {
// 	size_t	i;
// 	t_token	*temp;

// 	temp = *token_list;
// 	i = 0;
// 	while (compare_tk(ND_CMD, &temp))
// 	{
// 		i++;
// 		temp = temp->next;
// 	}
// 	return (i);
// }

// size_t	count_rd_node(t_token	**token_list)
// {
// 	size_t	i;
// 	t_token	*temp;

// 	temp = *token_list;
// 	i = 0;
// 	while (compare_tk(ND_REDIRECTS, &temp))
// 	{
// 		i++;
// 		temp = temp->next;
// 		if (!compare_tk(ND_CMD, &temp))
// 		{
// 			printf("count_rd_node:unexpexted token\n");
// 			return (i);
// 		}
// 		i++;
// 		temp = temp->next;
// 	}
// 	return (i);
// }

char	**parse_words(t_token **token_list)
{
	char	**ret;
	size_t		i;
	size_t	word_cnt;

	i = 0;
	ret = NULL;
	word_cnt = count_nodes(token_list, ND_CMD);
	if (word_cnt)
		ret = xmalloc((word_cnt + 1) * sizeof(char *));
	else
		return (ret);
	while (i < word_cnt)
	{
		if (!compare_tk(ND_CMD, token_list))
			break ; // syntax error
		ret[i] = ft_strdup((*token_list)->word);
		if (!ret[i])
		{
			free_str_array(ret);
			d_throw_error("parse_words", "strdup_error");
		}
		i++;
		*token_list = (*token_list)->next;
	}
	ret[i] = NULL;
	return (ret);
}

t_node	*parse_redirects(t_token **token_list)
{
	t_node	*node;

	node = NULL;
	if (compare_tk(ND_REDIRECTS, token_list))
	{
		node = create_node(ND_REDIRECTS);
		node->redirects = parse_redirect_arry(token_list);
	}
	return (node);
}

char	**parse_redirect_arry(t_token	**token_list)
{
	char	**ret;
	int		i;

	i = 0;
	ret = NULL;
	if (count_nodes(token_list, ND_REDIRECTS))
		ret = xmalloc((count_nodes(token_list, ND_REDIRECTS) + 1) * sizeof(char *));
	else
		return (ret);
	while (compare_tk(ND_REDIRECTS, token_list))
	{
		ret[i++] = ft_strdup((*token_list)->word);
		if (!ret[i - 1])
		{
			free_str_array(ret);
			d_throw_error("parse_redirect_arry", "strdup_error");
		}
		*token_list = (*token_list)->next;
		if (!compare_tk(ND_CMD, token_list))
			d_throw_error("parse_redirect_arry", "filename syntax error");
		// ret[i] = (*token_list)->word;
		ret[i++] = ft_strdup((*token_list)->word);
		if (!ret[i-1] )
		{
			free_str_array(ret);
			d_throw_error("parse_redirect_arry", "strdup_error");
		}
		*token_list = (*token_list)->next;
	}
	ret[i] = NULL;
	return (ret);
}

