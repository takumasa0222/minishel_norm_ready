/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_checker.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:14:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/10/26 20:24:32 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../libft/libft.h"
#include "../../includes/utils.h"

bool	is_metachar(int c)
{
	return (c && ft_strchr("|&;()<> \t\n", c));
}

bool	is_word(char *input)
{
	return (input && !is_metachar(*input));
}

bool	is_blank(int c)
{
	return (ft_strchr(" \t", c));
}

bool	is_operator(char *input)
{
	size_t		i;
	size_t		ope_len;
	static char	*ope[] = \
	{"||", "|", "&&", "&", ";;", ";", "(", ")", "|&", "\n", NULL};

	if (!input)
		d_throw_error("is_operator", "input is null");
	i = 0;
	ope_len = get_char_arry_size(ope);
	while (i < ope_len)
	{
		if (!ft_strncmp(ope[i], input, ft_strlen(ope[i])))
			return (true);
		i++;
	}
	return (false);
}
