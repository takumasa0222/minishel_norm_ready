/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extend_string_functions.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 01:22:21 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/14 02:10:07 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/utils.h"

static size_t	xlen_check(size_t len, size_t s_len, unsigned int st, char *s);

char	*x_strjoin(char const *s1, char const *s2)
{
	size_t			s1_len;
	size_t			s2_len;
	char			*ret;
	unsigned int	i;
	unsigned int	j;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	ret = xmalloc((s1_len + s2_len + 1) * sizeof(char));
	i = 0;
	while (i < s1_len)
	{
		ret[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < s2_len)
		ret[i++] = s2[j++];
	ret[i] = '\0';
	return (ret);
}

char	*x_substr(char const *s, unsigned int start, size_t len)
{
	char			*ret;
	unsigned int	i;
	unsigned int	j;
	size_t			str_len;

	if (!s || len > SIZE_MAX)
		return (NULL);
	str_len = ft_strlen(s);
	len = xlen_check(len, str_len, start, (char *)s);
	ret = xmalloc(len * sizeof(char));
	i = 0;
	j = 0;
	while (i < start && i < (unsigned int)ft_strlen(s))
		i++;
	while (j < len && s[i] && j < len - 1)
	{
		ret[j] = s[i];
		j++;
		i++;
	}
	ret[j] = '\0';
	return (ret);
}

static size_t	xlen_check(size_t len, size_t s_len, unsigned int st, char *s)
{
	if (s_len <= st || len == 0)
		return (1);
	if (s_len < len)
	{
		if (*s == '\0')
			return (1);
		if (s_len < len - st)
			return (s_len - st + 1);
		return (s_len + 1);
	}
	if (st + len > s_len)
		return (s_len - st + 1);
	return (len + 1);
}

char	*x_strdup(const char *s1)
{
	size_t	src_len;
	char	*ret;

	if (!s1)
		return (NULL);
	src_len = ft_strlen(s1);
	ret = xmalloc((src_len + 1) * sizeof (char));
	if (!ret)
		return (NULL);
	while (*s1)
		*ret++ = *s1++;
	*ret = '\0';
	return (ret - src_len);
}
