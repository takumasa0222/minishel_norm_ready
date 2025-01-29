/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extend_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:49:43 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/27 01:40:08 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"

static int		x_get_sep_cnt(char const *str, char dlm);
static char		*x_push_word(char const *str, int size);
static int		x_validate_words(char **mem, int cnt);
static int		x_ft_is_not_chr(char const *str, char c);

char	**x_split(char const *s, char c)
{
	char	**ans;
	int		size;
	int		i;
	int		k;

	ans = (char **)xmalloc(sizeof (char *) * (x_get_sep_cnt(s, c) + 1));
	i = 0;
	k = 0;
	while (s[i])
	{
		size = x_ft_is_not_chr(&(s[i]), c);
		i = i + size;
		if (size != 0 && k <= (x_get_sep_cnt(s, c)))
			ans[k++] = x_push_word(&(s[i - size]), size);
		if (x_validate_words(ans, k - 1))
			return (NULL);
		size = 0;
		while ((s[i] == c) && s[i])
			i++;
	}
	ans[k] = NULL;
	return (ans);
}

static int	x_get_sep_cnt(char const *str, char dlm)
{
	int	i;
	int	sp_cnt;

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

static int	x_validate_words(char **mem, int cnt)
{
	int				i;

	i = 0;
	if (!mem)
		return (1);
	if (cnt < 0)
		return (0);
	if (mem[cnt] == NULL)
	{
		while (i <= cnt)
		{
			free(mem[i]);
			i++;
		}
		free(mem);
		return (1);
	}
	return (0);
}

static int	x_ft_is_not_chr(char const *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (i);
	while (str[i] != c && str[i])
		i++;
	return (i);
}

static char	*x_push_word(char const *str, int size)
{
	int		i;
	char	*ret;

	i = 0;
	ret = xmalloc(sizeof(char) * (size + 1));
	while (i < size && str)
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
