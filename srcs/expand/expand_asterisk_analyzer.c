/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk_analyzer.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:25:43 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 17:26:15 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"
#include "../../includes/expand_asterisk.h"

void	analyze_aster_loop(t_cmp_str ***ret, char *line, size_t *x)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == ASTERISK)
		{
			if (i && is_first_aster(line, i))
				(*ret)[j++] = create_cmp_str(line, 0, i, PREFIX);
			j = analyze_sb_loop(ret, line, i, j);
			break ;
		}
		else
			i++;
	}
	*x = j;
}

size_t	analyze_sb_loop(t_cmp_str ***ret, char *line, size_t i, size_t j)
{
	size_t	nxt_ast;

	while (line[i])
	{
		i = skip_aster_sequence(line, i);
		nxt_ast = find_next_aster(line, i);
		if (!nxt_ast && i != ft_strlen(line))
		{
			(*ret)[j++] = create_cmp_str(line, i, ft_strlen(line), SUFFIX);
			break ;
		}
		else if (!nxt_ast)
			break ;
		else
			(*ret)[j++] = create_cmp_str(line, i, nxt_ast - i, CONTAIN);
		i = nxt_ast;
	}
	return (j);
}

size_t	skip_aster_sequence(char *line, size_t current)
{
	if (!line)
		throw_unexpected_error("is_first_aster", NULL);
	while (line[current] == ASTERISK)
		current++;
	return (current);
}

size_t	find_next_aster(char *line, size_t i)
{
	while (line[i])
	{
		if (line[i] == ASTERISK)
			return (i);
		i++;
	}
	return (0);
}
