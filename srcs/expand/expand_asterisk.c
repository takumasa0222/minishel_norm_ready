/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:23:01 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/30 03:11:21 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"
#include "../../includes/expand_asterisk.h"

static size_t	get_split_count(char const *str, char dlm);

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
		if (line[i] == ASTERISK)
		{
			is_aster_only_flg = true;
			i++;
		}
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
		if (str[j] == ASTERISK)
			return (false);
		j++;
	}
	return (true);
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

t_cmp_str	**init_cmp_str_arry(char *line)
{
	t_cmp_str	**ret;
	size_t		split_cnt;

	split_cnt = get_split_count(line, ASTERISK);
	ret = xmalloc(sizeof(t_cmp_str *) * (split_cnt + 1));
	return (ret);
}

static size_t	get_split_count(char const *str, char dlm)
{
	size_t	i;
	size_t	sp_cnt;

	i = 0;
	if (!str || !str[i])
		return (0);
	if (ft_strlen(str) == 1)
		return (1);
	sp_cnt = 0;
	while (str[i])
	{
		while (str[i] == dlm && str[i])
			i++;
		if (!str[i])
			break ;
		while (str[i] != dlm && str[i])
			i++;
		sp_cnt++;
	}
	return (sp_cnt);
}

t_cmp_str	**create_cmp_str_arry(char *line)
{
	t_cmp_str	**ret;
	size_t		i;

	if (!line)
		throw_unexpected_error("is_aster_only", "line is NULL");
	i = 0;
	ret = init_cmp_str_arry(line);
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
	// size_t	nxt_ast;

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
