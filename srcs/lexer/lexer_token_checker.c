/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_checker.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:14:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/08 21:16:25 by ssoeno           ###   ########.fr       */
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

bool	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool	is_operator(char *input)
{
	size_t		i;
	t_ope_map	ope_map[OPE_MAP_SIZE];

	if (!input)
		d_throw_error("is_operator_or_metacharacter", "input is null");
	i = 0;
	get_ope_map(ope_map);
	while (ope_map[i].op_str && ope_map[i].kind != ND_EOF)
	{
		if (!ft_strncmp(ope_map[i].op_str, input, ft_strlen(ope_map[i].op_str)))
			return (true);
		i++;
	}
	return (false);
}
