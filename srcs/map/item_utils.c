/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:53:20 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/24 19:28:33 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"

t_item	*item_new(char *name, char *value)
{
	t_item	*item;

	item = xmalloc(sizeof(*item));
	item->name = name;
	item->value = value;
	return (item);
}

char	*item_get_string(t_item *item)
{
	size_t	strsize;
	char	*str;

	if (!item || !item->name)
	{
		d_throw_error("item_get_string", "item or item->name is NULL");
		return (NULL);
	}
	strsize = ft_strlen(item->name) + 2;
	if (item->value)
		strsize += ft_strlen(item->value);
	str = xmalloc(strsize);
	ft_strlcpy(str, item->name, strsize);
	if (item->value)
	{
		ft_strlcat(str, "=", strsize);
		ft_strlcat(str, item->value, strsize);
	}
	return (str);
}
// 1 byte for "=" and 1 byte for null terminator

int	ft_strcmp_for_map(const char *s1, const char *s2)
{
	if (s1 == NULL || s2 == NULL)
	{
		return (ERROR);
	}
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
