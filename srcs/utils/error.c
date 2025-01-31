/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 01:33:45 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 21:25:00 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

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
	ft_putstr_fd(ERROR_PREFIX, STDERR_FILENO);
	ft_putendl_fd(err_msg, STDERR_FILENO);
	if (err_msg2)
	{
		ft_putstr_fd(ERROR_PREFIX, STDERR_FILENO);
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
