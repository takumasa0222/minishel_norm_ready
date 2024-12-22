/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_word.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 04:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/22 19:50:34 by ssoeno           ###   ########.fr       */
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
	while (temp && temp->kind == kind)
	{
		count++;
		temp = temp->next;
		if (kind == ND_REDIRECTS)
		{
			if (!temp || temp->kind != ND_CMD)
				break ;
			count++;
			temp = temp->next;
		}
	}
	return (count);
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

	word = ft_strdup((*token_list)->word);
	if (!word)
		d_throw_error("parse_single_redirect", "strdup_error");
	*token_list = (*token_list)->next;
	return (word);
}

void	handle_redirect_error(char **ret, size_t count)
{
	free_wordlist(&ret[count - 1]);
	d_throw_error("parse_redirect_array", "redirect syntax error");
}

char	**parse_redirect_arry(t_token **token_list)
{
	char	**ret;
	int		i;
	size_t	rd_cnt;

	ret = NULL;
	i = 0;
	rd_cnt = count_nodes(token_list, ND_REDIRECTS);
	if (!rd_cnt)
		return (NULL);
	ret = xmalloc((rd_cnt + 1) * sizeof(char *));
	while (compare_tk(ND_REDIRECTS, token_list))
	{
		ret[i++] = parse_single_redirect(token_list);
		if (!compare_tk(ND_CMD, token_list))
			handle_redirect_error(ret, i);
		ret[i++] = parse_single_redirect(token_list);
	}
	ret[i] = NULL;
	return (ret);
}
