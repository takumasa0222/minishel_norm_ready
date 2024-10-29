/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:33:29 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/10/26 16:43:59 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
/*
This function for testing purpose. When you submit this code, this should be
replased
*/

void	d_throw_error(char *func_name, char *error_msg)
{
	printf("Error: function name: %s, Error message: %s", func_name, error_msg);
	exit(EXIT_FAILURE);
}
