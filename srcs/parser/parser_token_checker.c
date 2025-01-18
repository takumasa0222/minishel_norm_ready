/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_checker.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 04:41:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/18 21:04:54 by tamatsuu         ###   ########.fr       */
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
