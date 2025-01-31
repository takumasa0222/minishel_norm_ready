/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 03:12:31 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 21:16:15 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"

char	*remove_quotes(char *str)
{
	size_t	i;
	size_t	quote_end;
	size_t	j;
	char	*ret;

	if (!str)
		throw_unexpected_error("remove_quotes", "str is null");
	ret = xmalloc(ft_strlen(str));
	i = 0;
	j = 0;
	while (str[i])
	{
		if (is_s_quote(str[i]) || is_d_quote(str[i]))
		{
			quote_end = move_to_next_quotation_expnd(str, i);
			while (++i < quote_end)
				ret[j++] = str[i];
			i++;
		}
		else
			ret[j++] = str[i++];
	}
	return (ret[j] = '\0', ret);
}

int	move_to_next_quotation_expnd(char *input, int i)
{
	if (is_s_quote(input[i]))
	{
		i++;
		while (input[i] && !is_s_quote(input[i]))
			i++;
		if (!input[i])
			throw_unexpected_error("mv_t_nxt_qttn_expnd", "s not closed");
	}
	else if (is_d_quote(input[i]))
	{
		i++;
		while (input[i] && !is_d_quote(input[i]))
			i++;
		if (!input[i])
			throw_unexpected_error("mv_t_nxt_qttn_expnd", "dquote not closed");
	}
	return (i);
}
