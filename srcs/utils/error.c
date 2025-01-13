/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:33:29 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/13 19:00:45 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#define ERROR_PREFIX "minishell: "
/*
This function for testing purpose. When you submit this code, this should be
replased
*/

void	d_throw_error(char *func_name, char *error_msg)
{
	printf("Error: function name: %s, Error message: %s", func_name, error_msg);
	exit(EXIT_FAILURE);
}

void	throw_unexpected_error(char *func_name, char *err_msg)
{
	static char	*default_err_msg = "Logically unexpected error";

	if (!err_msg)
		printf("Error: function name: %s, Error message: %s", \
		func_name, default_err_msg);
	else
		printf("Error: function name: %s, Error message: %s", \
		func_name, err_msg);
	exit(EXIT_FAILURE);
}

void	perror_prefix(void)
{
	ft_putstr_fd(ERROR_PREFIX, STDERR_FILENO);
}

void	builtin_error(char *func, char *name, char *err)
{
	perror_prefix();
	ft_putstr_fd(func, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (name)
	{
		ft_putstr_fd("`", STDERR_FILENO);
		ft_putstr_fd(name, STDERR_FILENO);
		ft_putstr_fd("': ", STDERR_FILENO);
	}
	ft_putstr_fd(err, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}
