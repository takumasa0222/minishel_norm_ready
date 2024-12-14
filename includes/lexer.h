/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 01:13:23 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/25 20:59:55 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdio.h>
# include <stdbool.h>

typedef struct s_token	t_token;
typedef enum e_token_kind
{
	TK_WORD,
	TK_RESERVED,
	TK_OP,
	TK_EOF,
}	t_token_kind;

typedef enum e_node_kind{
	ND_REDIRECTS,
	ND_IN_RD,
	ND_OUT_RD,
	ND_APPEND_RD,
	ND_HERE_DOC,
	ND_PIPE,
	ND_OR_OP,
	ND_AND_OP,
	ND_L_PARE,
	ND_R_PARE,
	ND_SUB_SHELL,
	ND_CMD,
	ND_FD_NUM,
	ND_FD_WORD
}	t_node_kind;

struct s_token {
	char			*word;
	t_node_kind		kind;
	t_token			*next;
};

t_token	*lexer(char *line);
bool	skip_blank(char **rest, char *input);
t_token	*fetch_fst_ope_token(char *input);
t_token	*fetch_fst_word_token(char *input);
int		move_to_next_quotation(char *input, int i);

bool	is_metachar(int c);
bool	is_word(char *input);
bool	is_blank(char c);
bool	is_operator(char *input);

bool	is_s_quote(char input);
bool	is_d_quote(char input);

size_t	get_char_arry_size(char **str);
t_token	*create_token(char *word, t_token_kind kind);

#endif