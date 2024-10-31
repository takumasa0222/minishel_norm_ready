/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:19:21 by ssoeno            #+#    #+#             */
/*   Updated: 2024/10/31 21:40:13 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_exit(int argc, char *argv[])
{
	if (argc != 1)
	{
		printf("%s: too many arguments\n", argv[0]);
		return (1);
	}
	exit(0);
}
