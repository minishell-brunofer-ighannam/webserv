/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LexerBuilder.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 00:29:16 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/05/31 16:44:47 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_BUILDFER_HPP
# define LEXER_BUILDFER_HPP

# include <iostream>

# include "Lexer.hpp"
# include "LexerIterator.hpp"

class LexerBuilder
{
private:
	Lexer	_lexer;
public:
	LexerBuilder() {};
	~LexerBuilder() {};

	LexerBuilder	&withFile(const std::string file_name) { _lexer.setFile(file_name); return *this; }
	LexerBuilder	&withDefaultTokens() {
		_lexer.addComment("#");
		_lexer.addDelimitter("{", LexerTokenType::LBRACE);
		_lexer.addDelimitter("}", LexerTokenType::RBRACE);
		_lexer.addDelimitter(";", LexerTokenType::SEMICOLON);
		_lexer.addQuote("\"");
		_lexer.addQuote("'");
		return *this;
	}
	LexerBuilder	&withComment(const std::string &comment) { _lexer.addComment(comment); return *this; }
	LexerBuilder	&withQuote(const std::string &quote) { _lexer.addQuote(quote); return *this; }
	LexerBuilder	&withKeyword(const std::string &keyword, LexerTokenType type) { _lexer.addDelimitter(keyword, type); return *this; }

	LexerIterator	build()
	{
		_lexer.run();
		return LexerIterator(_lexer);
	}
};

#endif
