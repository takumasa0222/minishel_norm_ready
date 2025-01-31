/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:15:53 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 17:18:02 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"
#include "../../includes/expand_asterisk.h"

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

void	free_cmp_arry(t_cmp_str ***cmp_arry)
{
	size_t	i;

	i = 0;
	if (!cmp_arry || !*cmp_arry)
		return ;
	while ((*cmp_arry)[i])
	{
		free((*cmp_arry)[i]->cmp_str);
		free((*cmp_arry)[i]);
		i++;
	}
	free(*cmp_arry);
}

size_t	get_split_count(char const *str, char dlm)
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
