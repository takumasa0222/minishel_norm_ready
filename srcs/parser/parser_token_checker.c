/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_checker.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 04:41:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/25 21:35:39 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

bool	match_token(t_node_kind expected, t_token **cur_token)
{
	if (!*cur_token)
		return (false);
	if ((*cur_token)->kind == expected)
	{
		*cur_token = (*cur_token)->next;
		return (true);
	}
	return (false);
}

bool	compare_tk(t_node_kind expected, t_token **cur_token)
{
	if (!*cur_token)
		return (false);
	return ((*cur_token)->kind == expected);
}

bool	is_cmd_parsable_tk(t_token **cur_token)
{
	if (!*cur_token)
		return (false);
	if (compare_tk(ND_CMD, cur_token) \
	|| compare_tk(ND_REDIRECTS, cur_token) \
	|| compare_tk(ND_L_PARE, cur_token))
		return (true);
	else
		return (false);
}

bool	is_cmd_tail_tk(t_token **cur_token)
{
	if (!*cur_token)
		return (false);
	if (compare_tk(ND_REDIRECTS, cur_token) \
	|| compare_tk(ND_PIPE, cur_token) \
	|| compare_tk(ND_AND_OP, cur_token) \
	|| compare_tk(ND_OR_OP, cur_token) \
	|| compare_tk(ND_EOF, cur_token) \
	|| compare_tk(ND_R_PARE, cur_token))
		return (true);
	else
		return (false);
}

bool	is_rnd_bracket_closed(t_token **cur_token)
{
	int		l_pare;
	t_token	*temp;

	if (!cur_token || !*cur_token)
		return (false);
	temp = *cur_token;
	l_pare = 0;
	while (temp)
	{
		if (compare_tk(ND_L_PARE, &temp))
			l_pare++;
		else if (compare_tk(ND_R_PARE, &temp))
			l_pare--;
		if (l_pare < 0)
			return (false);
		temp = temp->next;
	}
	return (!l_pare);
}
