/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 03:08:18 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/13 16:50:19 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"

size_t	retrieve_val_in_sq(char **ret, char *str, size_t i)
{
	size_t	closing_sq_pos;

	if (!is_s_quote(str[i]) || !ret || !str)
		d_throw_error("retrieve_val_in_sq", "arg is invalid");//unexpected error
	closing_sq_pos = skip_s_quote_block(str, i);
	append_substring(ret, str, i, closing_sq_pos - i + 1);
	if (!is_s_quote(str[closing_sq_pos]))
		d_throw_error("retrieve_val_in_sq", "unexpected error");//unexpected error
	return (closing_sq_pos);
}

size_t	retrieve_val_in_dq(char **ret, char *str, size_t i, t_context *ctx)
{
	size_t	j;

	if (!is_d_quote(str[i]) || !ret || !str)
		d_throw_error("retrieve_val_in_dq", "arg is invalid");//unexpected error
	j = i + 1;
	while (str[j] && !is_d_quote(str[j]))
	{
		if (is_dollar_symbol(str[j]))
		{
			append_substring(ret, str, i, j - i);
			j = retrieve_var(ret, str, j, ctx);
			i = j + 1;
		}
		j++;
	}
	append_substring(ret, str, i, j - i + 1);
	if (!is_d_quote(str[j]))
		d_throw_error("retrieve_val_in_dq", "unexpected error");//unexpected error
	return (j);
}

size_t	retrieve_var(char **ret, char *str, size_t i, t_context *ctx)
{
	size_t	var_name_len;
	char	*tmp;
	char	*expanded_val;

	if (!str)
		d_throw_error("retrieve_var", "arg is invalid");//unexpected error
	if (!str[i + 1])
		return (append_substring(ret, str, i, 1), i);
	var_name_len = retrieve_var_name_len(str, i + 1, false);
	if (!var_name_len)
		return (append_substring(ret, str, i, 1), i);
	tmp = ft_substr(str, i + 1, var_name_len);//FIX xmalloc should be used
	if (!tmp)
		d_throw_error("retrieve_var", "substr failed");//unexpected error
	if (var_name_len == 1 && !ft_strcmp(tmp, QUESTION_MARK))
		expanded_val = ft_itoa(ctx->last_status);//FIX xmalloc should be used
	else
		expanded_val = ft_strdup(map_get(ctx->env, tmp));//FIX xmalloc should be used
	if (expanded_val)
		append_substring(ret, expanded_val, 0, ft_strlen(expanded_val));
	else
		append_substring(ret, "", 0, 0);
	free(tmp);
	free(expanded_val);
	return (var_name_len + i);
}

size_t	retrieve_var_name_len(char *str, size_t i, bool is_heredoc)
{
	size_t	len;

	len = 0;
	if (!str)
		d_throw_error("retrieve_var_name_end", "arg is invalid");//unexpected error
	if (!is_heredoc && (is_d_quote(str[i]) || is_s_quote(str[i])))
		return (0);
	if (!ft_isalpha(str[i]) && str[i] != UNDER_SCORE)
		return (1);
	while (ft_isalnum(str[i + len]) || str[i + len] == UNDER_SCORE)
		len++;
	return (len);
}

size_t	retrieve_normal_val(char **ret, char *str, size_t i, bool is_heredoc)
{
	size_t	j;

	if (!str)
		d_throw_error("retrieve_normal_val", "arg is invalid");
	j = 0;
	if (is_heredoc)
	{
		while (str[i + j] && !is_dollar_symbol(str[i + j]))
			j++;
	}
	else
	{
		while (str[i + j] && !is_dollar_symbol(str[i + j]) \
		&& !is_d_quote(str[i + j]) && !is_s_quote(str[i + j]))
		j++;
	}
	append_substring(ret, str, i, j);
	return (i + j - 1);
}
