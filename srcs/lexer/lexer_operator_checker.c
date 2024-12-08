/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operator_checker.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:44:17 by ssoeno            #+#    #+#             */
/*   Updated: 2024/12/08 18:52:02 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../libft/libft.h"
#include "../../includes/utils.h"

void get_ope_map(t_ope_map *ope_map)
{
	size_t	i;

	t_ope_map tmp_map[] = {
		{"||", ND_OR_OP},
		{"&&", ND_AND_OP},
		{"|", ND_PIPE},
		{"<<", ND_REDIRECTS},
		{">>", ND_REDIRECTS},
		{"<", ND_REDIRECTS},
		{">", ND_REDIRECTS},
		{"(", ND_L_PARE},
		{")", ND_R_PARE},
		// {"\n", ND_NEWLINE},
		{NULL, ND_EOF}
	};
	i = 0;
	while(tmp_map[i].ope_str != NULL || tmp_map[i].kind != ND_EOF)
	{
		ope_map[i] = tmp_map[i];
		i++;
	}
	ope_map[i] = tmp_map[i];
}

t_token	*fetch_fst_ope_token(char *input)
{
	t_ope_map	ope_map[10];
	size_t		i;
	t_node_kind	kind;
	const char	*matched_operator;

	if (!input)
		d_throw_error("fetch_fst_ope_token", "input is NULL");
	i = 0;
	get_ope_map(ope_map);
	while (ope_map[i].ope_str)
	{
		if (!ft_strncmp(ope_map[i].ope_str, input, ft_strlen(ope_map[i].ope_str)))
		{
			matched_operator = ope_map[i].ope_str;
			kind = ope_map[i].kind;
			return (create_token(ft_strdup(ope_map->ope_str), ope_map[i].kind));
		}
		i++;
	}
	d_throw_error("fetch_fst_ope_token", "logically unexpected error");
	return (NULL);
}
