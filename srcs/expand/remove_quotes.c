/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 03:12:31 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/31 03:14:13 by tamatsuu         ###   ########.fr       */
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
		d_throw_error("remove_quotes", "unexpected error. str is null");
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
			d_throw_error("move_to_next_quotation", "squote not closed");
	}
	else if (is_d_quote(input[i]))
	{
		i++;
		while (input[i] && !is_d_quote(input[i]))
			i++;
		if (!input[i])
			d_throw_error("move_to_next_quotation", "dquote not closed");
	}
	return (i);
}
