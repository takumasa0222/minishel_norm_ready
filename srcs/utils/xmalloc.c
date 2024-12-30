/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 18:27:04 by ssoeno            #+#    #+#             */
/*   Updated: 2024/12/22 19:05:56 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

void	*xmalloc(size_t size)
{
	void	*p;

	p = malloc(size);
	if (!p)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (p);
}
