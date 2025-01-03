/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operator_checker.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:44:17 by ssoeno            #+#    #+#             */
/*   Updated: 2024/12/08 21:20:51 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../libft/libft.h"
#include "../../includes/utils.h"

void	get_ope_map(t_ope_map *ope_map)
{
	size_t				i;
	static t_ope_map	tmp_map[OPE_MAP_SIZE] = {
	{"||", ND_OR_OP},
	{"&&", ND_AND_OP},
	{"|", ND_PIPE},
	{"<<", ND_REDIRECTS},
	{">>", ND_REDIRECTS},
	{"<", ND_REDIRECTS},
	{">", ND_REDIRECTS},
	{"(", ND_L_PARE},
	{")", ND_R_PARE},
	{NULL, ND_EOF}
	};

	i = 0;
	while (i < OPE_MAP_SIZE)
	{
		ope_map[i] = tmp_map[i];
		i++;
	}
}

t_token	*fetch_fst_ope_token(char *input)
{
	t_ope_map	ope_map[OPE_MAP_SIZE];
	size_t		i;
	t_token		*ret;

	if (!input)
		d_throw_error("fetch_fst_ope_token", "input is NULL");
	i = 0;
	get_ope_map(ope_map);
	while (ope_map[i].op_str)
	{
		if (!ft_strncmp(ope_map[i].op_str, input,
				ft_strlen(ope_map[i].op_str)))
		{
			ret = create_token(ft_strdup(ope_map[i].op_str), ope_map[i].kind);
			return (ret);
		}
		i++;
	}
	d_throw_error("fetch_fst_ope_token", "logically unexpected error");
	return (NULL);
}
