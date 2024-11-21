/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_quote_checker.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 05:21:17 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/22 04:47:05 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

bool	is_s_quote(char input)
{
	return (input == '\'');
}

bool	is_d_quote(char input)
{
	return (input == '\"');
}
