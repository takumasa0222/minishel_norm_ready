/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_asterisk_checker.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:06:27 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 17:07:33 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"
#include "../../includes/expand_asterisk.h"

bool	match_aster_regex(t_item *file, t_cmp_str **cmp_arry, bool is_dot)
{
	size_t	i;
	size_t	j;
	char	is_match;

	is_match = true;
	if (!cmp_arry || !file)
		throw_unexpected_error("match_aster_regex", NULL);
	i = 0;
	j = 0;
	while (cmp_arry[i])
	{
		if (!j && cmp_arry[i]->cmp_type == PREFIX)
			is_match = match_prefix(file->name, &j, cmp_arry[i]->cmp_str);
		else if (cmp_arry[i]->cmp_type == SUFFIX)
			is_match = match_suffix(file, &j, cmp_arry[i]->cmp_str, is_dot);
		else if (cmp_arry[i]->cmp_type == CONTAIN)
			is_match = \
			match_contain(file->name, &j, cmp_arry[i]->cmp_str, is_dot);
		else if (cmp_arry[i]->cmp_type == ALL)
			is_match = file->name[0] != '.';
		if (!is_match)
			return (is_match);
		i++;
	}
	return (is_match);
}

bool	match_prefix(char *f_name, size_t *i, char *prefix)
{
	size_t	j;

	j = 0;
	while (j < ft_strlen(prefix))
	{
		if (!f_name[j] || !(f_name[j] == prefix[j]))
			return (false);
		j++;
	}
	*i = j;
	return (true);
}

bool	match_suffix(t_item *f, size_t *i, char *suffix, bool is_dot)
{
	size_t	j;
	size_t	f_len;
	size_t	suffix_len;

	if (!is_dot && f->name[0] == '.')
		return (false);
	j = 0;
	f_len = ft_strlen(f->name);
	suffix_len = ft_strlen(suffix);
	if (suffix[suffix_len - 1] == '/')
		return (match_suffix_directory(f, i, suffix));
	if (*i >= f_len || f_len < suffix_len + *i)
		return (false);
	while (j <= suffix_len && f->name[f_len - j] == suffix[suffix_len - j])
	{
		if (f_len < *i + j)
			return (false);
		if (!suffix[j])
			return (true);
		j++;
	}
	*i = j;
	return (false);
}

bool	match_suffix_directory(t_item *f, size_t *i, char *suffix)
{
	size_t	j;
	size_t	f_len;
	size_t	suffix_len;

	f_len = ft_strlen(f->name);
	suffix_len = ft_strlen(suffix);
	if (ft_strcmp(f->value, "DIR"))
		return (false);
	if (suffix_len == 1)
		return (true);
	if (*i >= f_len || f_len < suffix_len + *i)
		return (false);
	j = 2;
	while (j <= suffix_len && f->name[f_len - j + 1] == suffix[suffix_len - j])
	{
		if (f_len < *i + j)
			return (false);
		if (!suffix[j])
			return (true);
		j++;
	}
	return (false);
}

bool	match_contain(char *f_name, size_t *i, char *contain, bool is_dot)
{
	size_t	k;
	size_t	contain_len;

	if (!is_dot && f_name[0] == '.')
		return (false);
	k = 0;
	contain_len = ft_strlen(contain);
	if (*i >= ft_strlen(f_name) || ft_strlen(f_name) < contain_len + *i)
		return (false);
	while (f_name[*i])
	{
		k = 0;
		while (f_name[*i] == contain[k])
		{
			if (k == contain_len - 1)
				return ((*i)++, true);
			if (!f_name[*i])
				return (false);
			(*i)++;
			k++;
		}
		*i = *i - k + 1;
	}
	return (false);
}
