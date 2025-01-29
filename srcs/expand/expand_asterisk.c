/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:23:01 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/29 14:27:29 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"

typedef enum e_cmp_type{
	PREFIX,
	SUFFIX,
	CONTAIN,
	ALL
}	t_cmp_type;

typedef struct s_cmp_str
{
	char			*compared_str;
	t_cmp_type		compare_type;
}	t_cmp_str;

bool	is_aster_only(char *line)
{
	size_t	i;
	bool	is_aster_only_flg;

	i = 0;
	is_aster_only_flg = false;
	if (!line)
		throw_unexpected_error("is_aster_only", "line is NULL");
	while (line[i])
	{
		if (line[i] == '*')
			is_aster_only_flg = true;
		else
		{
			is_aster_only_flg = false;
			break ;
		}
	}
	return (is_aster_only_flg);
}

t_cmp_str	*create_cmp_str(char *s, size_t st, size_t len, t_cmp_type typ)
{
	t_cmp_str	*ret;

	ret = xmalloc(sizeof(t_cmp_str));
	ret->compared_str = x_substr(s, (unsigned int)st, len);
	ret->compare_type = typ;
	return (ret);
}

bool	is_first_aster(char *str, size_t i)
{
	size_t	j;

	if (i < 1 || !str)
		throw_unexpected_error("is_first_aster", NULL);
	j = 0;
	while (j < i && str[j])
	{
		if (str[j] == '*')
			return (false);
		j++;
	}
	return (true);
}

size_t	skip_aster_sequence(char *line, size_t current)
{
	if (!line)
		throw_unexpected_error("is_first_aster", NULL);
	while (line[current] == '*')
		current++;
	return (current);
}


t_cmp_str	**create_compared_str_arry(char *line)
{
	t_cmp_str	**ret;
	size_t		i;

	if (!line)
		throw_unexpected_error("is_aster_only", "line is NULL");
	i = 0;
	ret = init_compared_str();
	if (is_aster_only(line))
		ret[i++] = create_cmp_str(line, 0, ft_strlen(line), ALL);
	else
		analyze_aster_loop(&ret, line, &i);
	ret[i] = NULL;
	return (ret);
}

void	analyze_aster_loop(t_cmp_str ***ret, char *line, size_t *x)
{
	size_t	i;
	size_t	j;
	size_t	nxt_ast;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == '*')
		{
			if (i && is_first_aster(line, i))
				(*ret)[j++] = create_cmp_str(line, 0, i, PREFIX);
			i = skip_aster_sequence(line, i);
			nxt_ast = find_next_aster(line, i);
			if (!nxt_ast)
				(*ret)[j++] = create_cmp_str(line, i, ft_strlen(line), SUFFIX);
			else
				(*ret)[j++] = create_cmp_str(line, i, nxt_ast, CONTAIN);
			i = nxt_ast + skip_aster_sequence(line, i + nxt_ast);
		}
		else
			i++;
	}
	*x = j;
}
