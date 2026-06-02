/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LexerTokenType.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunofer <brunofer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 11:51:17 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/02 15:31:25 by brunofer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_TOKEN_TYPE_HPP
# define LEXER_TOKEN_TYPE_HPP

# include <iostream>

enum LexerTokenType
{
	COMMENT			= '#',
	LBRACE			= '{',
	RBRACE			= '}',
	SEMICOLON		= ';',
	STRING_SINGLE	= '\'',
	STRING_DOUBLE	= '"',
	WORD			= 'w',
	ERROR			= 'e',
	END				= '\0',
};

std::ostream	&operator<<(std::ostream &os, LexerTokenType type)
{
	switch (type)
	{
	case LexerTokenType::COMMENT :
		os << "COMMENT";
		break;
	case LexerTokenType::LBRACE :
		os << "LBRACE";
		break;
	case LexerTokenType::RBRACE :
		os << "RBRACE";
		break;
	case LexerTokenType::SEMICOLON :
		os << "SEMICOLON";
		break;
	case LexerTokenType::STRING_SINGLE :
		os << "STRING_SINGLE";
		break;
	case LexerTokenType::STRING_DOUBLE :
		os << "STRING_DOUBLE";
		break;
	case LexerTokenType::WORD :
		os << "WORD";
		break;
	case LexerTokenType::ERROR :
		os << "ERROR";
		break;
	case LexerTokenType::END :
		os << "END";
		break;
	default:
		break;
	}
	return os;
}

#endif
