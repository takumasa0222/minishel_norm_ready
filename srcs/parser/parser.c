/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:51:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/05 03:30:22 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"

/*
this parser will create AST based on below eBNF.
<letter> ::= a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q | r | s | t | u | v | w | x | y | z |
A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z

<digit> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

<number> ::= <digit> | <number> <digit>

<word> ::= <letter> | <word> <letter> | <word> '_'

<redirection> ::= '>' <word> | '<' <word> | <number> '>' <word> | <number> '<' <word> | '>>' <word> | 
<number> '>>' <word> |
'<<' <word> |
<number> '<<' <word> |
<number> '<>' <word> |
'<>' <word>

<simple_command_element> ::= <word> | <redirection>

<redirection_list> ::= <redirection> | <redirection_list> <redirection>

<simple_command> ::= <simple_command_element> | <simple_command><simple_command_element>

<command> ::= <simple_command> | <shell_command> | <shell_command> <redirection_list>

<shell_command> ::= <subshell> | <list>

<subshell> ::= ‘(’ <list> ’)’

<list> ::= <list> ‘&&’ <list> | <list> ‘||’  <list>  | <pipeline_command>

<pipeline_command> ::= <pipeline_command> ‘|’  <pipeline_command> | <command>*/

t_node	*parser(t_token *token)
{
	
}