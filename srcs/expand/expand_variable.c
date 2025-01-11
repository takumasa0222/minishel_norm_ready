/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 02:58:35 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/11 02:55:58 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"
#include "../../get_next_line/get_next_line.h"

/*
Asterisk -> not implemented yet
$? -> not implemented yet?
Single quotes (suppress expansion): Detected using is_s_quote. 
	calls retrieve_val_in_sq which skips the single quotes and 
	returns the string inside the quotes.
Double quotes (allow expansion): Detected using is_d_quote. 
	calls retrieve_val_in_dq
	which expands the variables inside the quotes.
Dollar symbol (expand variables): Detected using is_dollar_symbol. 
	calls retrieve_var which expands the variables
	by looking up its value in envp. 
	If not found, returns an empty string.
Normal value: Handled by retrieve_normal_val
	append literal characters to ret until a special character (', ", $)
*/

char	*expand_variable(char *str, t_context *ctx)
{
	size_t	i;
	char	*ret;

	i = 0;
	ret = NULL;
	while (str[i])
	{
		if (is_s_quote(str[i]))
			i = retrieve_val_in_sq(&ret, str, i);
		else if (is_dollar_symbol(str[i]))
			i = retrieve_var(&ret, str, i, ctx);
		else if (is_d_quote(str[i]))
			i = retrieve_val_in_dq(&ret, str, i, ctx);
		else
			i = retrieve_normal_val(&ret, str, i, false);
		i++;
	}
	return (ret);
}

int	expand_heredoc_var(t_node *node, t_context *ctx)
{
	int		pipe_fds[2];
	char	*line;
	char	*ret;

	if (pipe(pipe_fds) == -1)
		d_throw_error("expand_heredoc_var", "pipe failed");
	line = get_next_line(node->fd_num);
	while (line)
	{
		ret = NULL;
		if (ft_strchr(line, DOLLAR))
			ret = expand_var_in_heredoc(line, ctx);
		if (ret)
			write(pipe_fds[1], ret, ft_strlen(ret));
		else
			write(pipe_fds[1], line, ft_strlen(line));
		free(line);
		free(ret);
		line = NULL;
		line = get_next_line(node->fd_num);
	}
	close(pipe_fds[1]);
	return (pipe_fds[0]);
}

char	*expand_var_in_heredoc(char *str, t_context *ctx)
{
	size_t	i;
	char	*ret;

	i = 0;
	ret = NULL;
	while (str[i])
	{
		if (is_dollar_symbol(str[i]))
			i = retrieve_var_in_heredoc(&ret, str, i, ctx);
		else
			i = retrieve_normal_val(&ret, str, i, true);
		i++;
	}
	return (ret);
}

size_t	retrieve_var_in_heredoc(char **ret, char *str, size_t i, t_context *ctx)
{
	size_t	var_name_len;
	char	*tmp;
	char	*expanded_val;

	if (!str)
		d_throw_error("retrieve_var", "arg is invalid");
	if (!str[i + 1])
		return (append_substring(ret, str, i, 1), i);
	var_name_len = retrieve_var_name_len(str, i + 1, true);
	if (!var_name_len)
		return (append_substring(ret, str, i, 1), i);
	tmp = ft_substr(str, i + 1, var_name_len);
	if (!tmp)
		d_throw_error("retrieve_var", "substr failed");
	if (var_name_len == 1 && !ft_strcmp(tmp, QUESTION_MARK))
		expanded_val = ft_itoa(ctx->last_status);
	else
		expanded_val = ft_strdup(map_get(ctx->env, tmp));
	if (expanded_val)
		append_substring(ret, expanded_val, 0, ft_strlen(expanded_val));
	else
		append_substring(ret, "", 0, 0);
	free(tmp);
	free(expanded_val);
	return (var_name_len + i);
}
