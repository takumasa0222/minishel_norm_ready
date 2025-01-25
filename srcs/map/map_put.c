/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_put.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:53:47 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/24 19:28:05 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"

int	split_name_value(const char *string, bool allow_empty_value, char **name,
		char **value)
{
	char	*name_end;

	if (!string || !name || !value)
		return (ERROR);
	name_end = ft_strchr(string, '=');
	if (name_end == NULL)
	{
		if (!allow_empty_value)
			return (ERROR);
		*name = x_strdup(string);
		*value = NULL;
	}
	else
	{
		*name = x_substr(string, 0, name_end - string);
		*value = x_strdup(name_end + 1);
	}
	return (EXIT_SUCCESS);
}

int	map_put(t_map *map, const char *string, bool allow_empty_value)
{
	int		result;
	char	*name;
	char	*value;

	if (!map || !string)
		return (ERROR);
	name = NULL;
	value = NULL;
	result = split_name_value(string, allow_empty_value, &name, &value);
	if (result != 0)
		return (result);
	result = map_set(map, name, value);
	free(name);
	free(value);
	return (result);
}
