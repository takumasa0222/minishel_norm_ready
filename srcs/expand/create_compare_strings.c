/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_compare_strings.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:09:29 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 21:16:47 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"
#include "../../includes/expand_asterisk.h"

t_cmp_str	**init_cmp_str_arry(char *line)
{
	t_cmp_str	**ret;
	size_t		split_cnt;

	split_cnt = get_split_count(line, ASTERISK);
	if (!split_cnt)
		split_cnt = 1;
	if (split_cnt > SIZE_MAX / sizeof(t_cmp_str *) - 1)
		throw_unexpected_error("init_cmp_str_arry", "allocationsize overflow");
	ret = xmalloc(sizeof(t_cmp_str *) * (split_cnt + 1));
	return (ret);
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

t_cmp_str	*create_cmp_str(char *s, size_t st, size_t len, t_cmp_type typ)
{
	t_cmp_str	*ret;

	ret = xmalloc(sizeof(t_cmp_str));
	ret->cmp_str = x_substr(s, (unsigned int)st, len);
	ret->cmp_type = typ;
	return (ret);
}
