/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:55:17 by ssoeno            #+#    #+#             */
/*   Updated: 2024/10/31 21:40:16 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_cd(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("%s: wrong argument\n", argv[0]);
		return (1);
	}
	if (chdir(argv[1]) < 0)
	{
		perror(argv[1]);
		return (1);
	}
	return (0);
}
