/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:49:20 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/19 19:15:43 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtin.h"

bool	is_no_newline_option(char *str)
{
	int	i;

	i = 1;
	if (!str || str[0] != '-')
		return (false);
	while (str[i] && str[i] == 'n')
	{
		i++;
		if (str[i] == '\0')
			return (true);
	}
	return (false);
}
/*
echo -nabc newline (expect -nabc newline)
*/

int	builtin_echo(int argc, char *argv[], t_map *envmap)
{
	bool	is_no_newline;
	size_t	i;

	(void)argc;
	(void)envmap;
	i = 1;
	is_no_newline = false;
	if (argv[1] && is_no_newline_option(argv[1]))
	{
		is_no_newline = true;
		i++;
	}
	while (argv[i])
	{
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		i++;
		if (argv[i])
			write(STDOUT_FILENO, " ", 1);
	}
	if (!is_no_newline)
		write(STDOUT_FILENO, "\n", 1);
	return (EXIT_SUCCESS);
}
