/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 02:46:35 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/05 03:19:46 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef enum e_node_kind{
	ND_IN_RD,
	ND_OUT_RD,
	ND_APPEND_RD,
	ND_HERE_DOC,
	ND_PIPE,
	ND_OR_OP,
	ND_AND_OP,
	ND_L_PARE,
	ND_R_PARE,
	ND_WORD,
	ND_FD_NUM
}	t_node_kind;

typedef struct s_node	t_node;

// AST node type
struct s_node {
	t_node_kind	kind;
	t_node		*lhs;
	t_node		*rhs;
	char		*val;
	int			num;
};

#endif