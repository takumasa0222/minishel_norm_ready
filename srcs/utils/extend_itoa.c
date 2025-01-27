/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extend_itoa.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 02:40:07 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/27 01:45:25 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"

static unsigned long	x_pstv_int_pow(unsigned int x, unsigned int y);
static int				x_get_digit_size(int n);
static unsigned int		x_abs_int(int i);

char	*x_itoa(int n)
{
	char	*ans;
	int		i;
	int		size;

	size = x_get_digit_size(n);
	ans = (char *)xmalloc((size + 1) * sizeof (char));
	i = 0;
	if (n < 0)
	{
		ans[i++] = '-';
		size = size - 1;
	}
	while (0 < size--)
	{
		if (n != 0)
			ans[i++] = x_abs_int(n) / x_pstv_int_pow(10, size) + 48;
		else
			ans[i++] = 48;
		if (n != 0)
			n = x_abs_int(n) % x_pstv_int_pow(10, size);
	}
	return (ans[i] = '\0', ans);
}

static unsigned int	x_abs_int(int i)
{
	unsigned int	ans;

	if (i < 0)
	{
		ans = i * -1;
		return (ans);
	}
	return ((unsigned int) i);
}

static unsigned long	x_pstv_int_pow(unsigned int x, unsigned int y)
{
	unsigned long	ans;
	unsigned int	i;

	i = 1;
	ans = (unsigned long)x;
	if (y == 0)
	{
		return (1);
	}
	while (i < y)
	{
		ans = ans * x;
		i++;
	}
	return (ans);
}

static int	x_get_digit_size(int n)
{
	long	i;
	int		dig_size;
	int		j;

	i = n;
	dig_size = 0;
	j = 0;
	if (i == 0)
		return (1);
	if (i < 0)
	{
		i = i * -1;
		dig_size = dig_size + 1;
	}
	if (i < 10)
		return (dig_size + 1);
	while (10 <= i / x_pstv_int_pow(10, j))
		j++;
	return (j + dig_size + 1);
}
