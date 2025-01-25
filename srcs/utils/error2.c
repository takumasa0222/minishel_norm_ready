/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 01:33:45 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/25 01:38:49 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#define ERROR_PREFIX "minishell: "

void	throw_system_error(char *err_msg, char *err_msg2)
{
	ft_putstr_fd(ERROR_PREFIX, STDERR_FILENO);
	ft_putendl_fd(err_msg, STDERR_FILENO);
	if (err_msg2)
	{
		ft_putstr_fd(ERROR_PREFIX, STDERR_FILENO);
		ft_putendl_fd(err_msg2, STDERR_FILENO);
	}
	exit (EXIT_FAILURE);
}
