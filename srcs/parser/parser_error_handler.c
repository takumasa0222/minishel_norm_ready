/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:53:46 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/25 21:35:59 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

void	parser_syntax_err(char ***ar, t_syntax_error *e, t_node **n, char *msg)
{
	if (!e->is_error)
	{
		e->is_error = true;
		e->err_msg = msg;
	}
	if (ar)
		free_wordlist(ar);
	if (n)
		free_ast(n);
}

void	log_syntax_err(char ***arry, t_syntax_error *err, t_node **node)
{
	if ((*node)->kind == ND_AND_OP)
		parser_syntax_err(arry, err, node, ERR_MSG_AND_OP);
	else
		parser_syntax_err(arry, err, node, ERR_MSG_OR_OP);
}
