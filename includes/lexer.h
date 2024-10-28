/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 01:13:23 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/10/29 01:36:00 by tamatsuu         ###   ########.fr       */
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

struct s_token {
	char			*word;
	t_token_kind	kind;
	t_token			*next;
};

t_token	*lexer(char *line);
bool	skip_blank(char **rest, char *input);
t_token	*fetch_fst_ope_token(char *input);
t_token	*fetch_fst_word_token(char *input);

bool	is_metachar(int c);
bool	is_word(char *input);
bool	is_blank(char c);
bool	is_operator(char *input);

size_t	get_char_arry_size(char **str);
t_token	*create_token(char *word, t_token_kind kind);
#endif
