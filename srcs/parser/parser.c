/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:51:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/07 05:15:05 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"

/*
this parser will create AST based on below eBNF.
Number ::= Digit | Number Digit
WordList ::= Word | WordList Word
Redirection ::= 
                  '>' Filename 
                | '<' Filename 
                | '>>' Filename 
                | '<<' Filename 
                | '<' Filename              
                | Number '>' Filename 
                | Number '<' Filename 
                | Number '>>' Filename 
                | Number '<<' Filename 
                | Number '<>' Filename 

Simple_command
         ::= Redirection* ( WordList Redirection* | Redirection+ )
           | WordList Redirection*
Command ::= Simple_command | Shell_command
Shell_command ::= Subshell | List
Subshell ::= '(' List ')'
List ::= List '&&' List | List '||' List | Pipeline_command
Pipeline_command ::= Pipeline_command '|' Pipeline_command | Command*/

t_node	*parser(t_token *token)
{
	
}