/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:40:23 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/14 02:00:05 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../libft/libft.h"
#include "../../includes/utils.h"

size_t	get_char_arry_size(char **str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (i);
	while (str[i])
		i++;
	return (i);
}

/* changed t_token_kind to t_node_kind*/
t_token	*create_token(char *word, t_node_kind kind)
{
	t_token	*ret;

	ret = xmalloc(1 * sizeof(t_token));
	ret->kind = kind;
	if (word)
		ret->word = word;
	else
		ret->word = NULL;
	ret->next = NULL;
	return (ret);
}
