/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_checker.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 04:41:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/22 04:47:38 by tamatsuu         ###   ########.fr       */
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

bool	compare_token(t_node_kind expected, t_token **cur_token)
{
	if (!*cur_token)
		return (false);
	return ((*cur_token)->kind == expected);
}
