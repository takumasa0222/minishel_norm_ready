#include "../../includes/minishell.h"
#include "../../includes/utils.h"

t_item	*item_new(char *name, char *value)
{
	t_item	*item;

	item = malloc(sizeof(*item));
	if (!item)
		d_throw_error("item_new", "Memory allocation error");
	item->name = ft_strdup(name);
	item->value = value;
	item->next = NULL;
	return (item);
}

char	*item_get_string(t_item *item)
{
	size_t	strsize;
	char	*str;

	strsize = ft_strlen(item->name) + 2;
	if (item->value)
		strsize += ft_strlen(item->value);
	str = malloc(strsize);
	if (str == NULL)
		d_throw_error("item_get_string", "Memory allocation error");
	ft_strlcpy(str, item->name, strsize);
	if (item->value)
	{
		ft_strlcat(str, "=", strsize);
		ft_strlcat(str, item->value, strsize);
	}
	return (str);
}
// 1 byte for "=" and 1 byte for null terminator

int ft_strcmp_for_map(const char *s1, const char *s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}
