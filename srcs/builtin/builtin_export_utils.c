/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:49:49 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/31 16:59:11 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"
#include "../../includes/map.h"

static t_map	*build_sorted_env(t_map *envmap);
static void		bubble_sort_map(t_map *map);
static void		swap_item(t_item *a, t_item *b);

void	print_sorted_env(t_map *envmap)
{
	t_item	*cur;
	t_map	*sorted_map;

	sorted_map = build_sorted_env(envmap);
	cur = sorted_map->item_head.next;
	while (cur)
	{
		if (cur->value)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(cur->name, STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(cur->value, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(cur->name, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		cur = cur->next;
	}
	free_map(&sorted_map);
}

static t_map	*build_sorted_env(t_map *envmap)
{
	t_map	*sorted_map;
	t_item	*cur;

	sorted_map = map_new();
	cur = envmap->item_head.next;
	while (cur)
	{
		map_add_item(sorted_map, cur->name, cur->value);
		cur = cur->next;
	}
	bubble_sort_map(sorted_map);
	return (sorted_map);
}

static void	bubble_sort_map(t_map *map)
{
	t_item	*cur;
	t_item	*next;

	cur = map->item_head.next;
	while (cur)
	{
		next = cur->next;
		while (next)
		{
			if (ft_strcmp(cur->name, next->name) > 0)
				swap_item(cur, next);
			next = next->next;
		}
		cur = cur->next;
	}
}

static void	swap_item(t_item *a, t_item *b)
{
	char	*tmp_name;
	char	*tmp_value;

	tmp_name = a->name;
	tmp_value = a->value;
	a->name = b->name;
	a->value = b->value;
	b->name = tmp_name;
	b->value = tmp_value;
}
