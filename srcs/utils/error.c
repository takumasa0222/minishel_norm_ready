/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 01:33:45 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/26 13:11:04 by ssoeno           ###   ########.fr       */
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

	ft_putstr_fd("Error: function name:", STDERR_FILENO);
	ft_putstr_fd(func_name, STDERR_FILENO);
	ft_putstr_fd(" Error message:", STDERR_FILENO);
	if (err_msg)
		ft_putendl_fd(err_msg, STDERR_FILENO);
	else
		ft_putendl_fd(default_err_msg, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	throw_syntax_error(char *err_msg, char *err_msg2)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putendl_fd(err_msg, STDERR_FILENO);
	if (err_msg2)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putendl_fd(err_msg2, STDERR_FILENO);
	}
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
