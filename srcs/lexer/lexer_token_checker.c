/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_checker.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 01:33:45 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/02/04 04:03:23 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../libft/libft.h"
#include "../../includes/utils.h"

bool	is_metachar(int c)
{
	return (c && ft_strchr("&|()<>; \t\n", c));
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
		throw_unexpected_error("is_operator", "input is null");
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

bool	is_unsupported(char *input, t_syntax_error *syntx_err)
{
	if (input && (*input == '&' || *input == ';'))
	{
		syntx_err->is_error = true;
		syntx_err->err_msg = ERR_MSG_UNSUPPORTED_SYNTAX;
		return (true);
	}
	return (false);
}
