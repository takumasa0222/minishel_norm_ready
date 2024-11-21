/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_word.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 04:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/22 04:48:25 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

char	**parse_words(t_token	**token_list)
{
	char	**ret;
	int		i;
	size_t	word_cnt;

	i = 0;
	ret = NULL;
	word_cnt = count_word_node(token_list);
	if (word_cnt)
		ret = malloc((word_cnt + 1) * sizeof(char *));
	else
		return (ret);
	if (!ret)
		d_throw_error("parse_words", "malloc_error");
	while (compare_token(ND_FD_WORD, token_list))
	{
		ret[i] = (*token_list)->word;
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
	if (compare_token(ND_REDIRECTS, token_list))
	{
		node = create_node(ND_REDIRECTS);
		node->redirects = parse_redirect_arry(token_list);
	}
	return (node);
}

size_t	count_word_node(t_token	**token_list)
{
	size_t	i;
	t_token	*temp;

	temp = *token_list;
	i = 0;
	while (compare_token(ND_FD_WORD, &temp))
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

char	**parse_redirect_arry(t_token	**token_list)
{
	char	**ret;
	int		i;

	i = 0;
	ret = NULL;
	if (count_rd_node(token_list))
		ret = malloc((count_rd_node(token_list) + 1) * sizeof(char *));
	else
		return (ret);
	if (!ret)
		d_throw_error("parse_redirect_arry", "malloc_error");
	while (compare_token(ND_REDIRECTS, token_list))
	{
		ret[i] = (*token_list)->word;
		i++;
		*token_list = (*token_list)->next;
		if (!compare_token(ND_FD_WORD, token_list))
			d_throw_error("parse_redirect_arry", "filename syntax error");
		ret[i] = (*token_list)->word;
		i++;
		*token_list = (*token_list)->next;
	}
	ret[i] = NULL;
	return (ret);
}

size_t	count_rd_node(t_token	**token_list)
{
	size_t	i;
	t_token	*temp;

	temp = *token_list;
	i = 0;
	while (compare_token(ND_REDIRECTS, &temp))
	{
		i++;
		temp = temp->next;
		if (!compare_token(ND_FD_WORD, &temp))
		{
			printf("count_rd_node:unexpexted token\n");
			return (i);
		}
		i++;
		temp = temp->next;
	}
	return (i);
}
