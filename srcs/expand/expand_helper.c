/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 21:14:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/30 04:17:36 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"

size_t	skip_s_quote_block(char *str, size_t i)
{
	if (!str)
		d_throw_error("skip_s_quote_block", "str is null");//unexpected error
	i = i + 1;
	while (str[i] && !is_s_quote(str[i]))
		i++;
	return (i);
}

bool	is_dollar_symbol(char c)
{
	return (c == DOLLAR);
}

char	*append_substring(char **ret, char *str, size_t i, size_t len)
{
	char	*tmp;
	char	*ret_val;

	tmp = x_substr(str, i, len);
	ret_val = x_strjoin(*ret, tmp);
	free(tmp);
	free(*ret);
	*ret = NULL;
	*ret = ret_val;
	return (ret_val);
}
