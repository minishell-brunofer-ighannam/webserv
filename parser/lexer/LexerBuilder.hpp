/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LexerBuilder.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 00:29:16 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/05/31 00:53:39 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_BUILDFER_HPP
# define LEXER_BUILDFER_HPP

# include <iostream>

# include "Lexer.hpp"
# include "LexerIterator.hpp"

enum ELexerTokenType
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

std::ostream	&operator<<(std::ostream &os, ELexerTokenType type)
{
	switch (type)
	{
	case ELexerTokenType::COMMENT :
		os << "COMMENT";
		break;
	case ELexerTokenType::LBRACE :
		os << "LBRACE";
		break;
	case ELexerTokenType::RBRACE :
		os << "RBRACE";
		break;
	case ELexerTokenType::SEMICOLON :
		os << "SEMICOLON";
		break;
	case ELexerTokenType::STRING_SINGLE :
		os << "STRING_SINGLE";
		break;
	case ELexerTokenType::STRING_DOUBLE :
		os << "STRING_DOUBLE";
		break;
	case ELexerTokenType::WORD :
		os << "WORD";
		break;
	case ELexerTokenType::ERROR :
		os << "ERROR";
		break;
	case ELexerTokenType::END :
		os << "END";
		break;
	default:
		break;
	}
	return os;
}

class LexerBuilder
{
private:
	Lexer<ELexerTokenType>	_lexer;
public:
	LexerBuilder() {};
	~LexerBuilder() {};

	LexerBuilder	&withFile(const std::string file_name) { _lexer.setFile(file_name); return *this; }
	LexerBuilder	&withDefaultTokens() {
		_lexer.addComment("#");
		_lexer.addKeyword("{", ELexerTokenType::LBRACE);
		_lexer.addKeyword("}", ELexerTokenType::RBRACE);
		_lexer.addKeyword(";", ELexerTokenType::SEMICOLON);
		_lexer.addQuote("\"");
		_lexer.addQuote("'");
		return *this;
	}
	LexerBuilder	&withComment(const std::string &comment) { _lexer.addComment(comment); return *this; }
	LexerBuilder	&withQuote(const std::string &quote) { _lexer.addQuote(quote); return *this; }
	LexerBuilder	&withKeyword(const std::string &keyword, ELexerTokenType type) { _lexer.addKeyword(keyword, type); return *this; }

	LexerIterator<ELexerTokenType>	build()
	{
		_lexer.run();
		return LexerIterator<ELexerTokenType>(_lexer);
	}
};

#endif
