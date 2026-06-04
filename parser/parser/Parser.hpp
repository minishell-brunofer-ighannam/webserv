/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 14:09:14 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/04 13:31:40 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <map>
# include <string>

# include "ParserToken.hpp"
# include "LexerBuilder.hpp"
# include "LexerIterator.hpp"
# include "ParserComposite.hpp"
# include "ParserAst.hpp"

class Parser
{
private:
	LexerBuilder							_lexer_builder;
	LexerIterator							_it;
	std::map<std::string, ParserTokenType>	_block_keywords;
	std::map<std::string, ParserTokenType>	_directive_keywords;
	std::map<std::string, ParserTokenType>	_modifiers;
	std::vector<std::string>				_errors;

	typedef	std::map<std::string, ParserTokenType>::iterator opts_iterator;

	void	_skipComments()
	{
		while (_it && *_it == LexerTokenType::COMMENT)
			++_it;
	}

	ParserToken	_convertToken(const LexerToken &token, bool after_location = false)
	{
		if (token == LexerTokenType::STRING_SINGLE) return ParserToken::fromLexerToken(token, ParserTokenType::PT_STRING_SINGLE);
		if (token == LexerTokenType::STRING_DOUBLE) return ParserToken::fromLexerToken(token, ParserTokenType::PT_STRING_DOUBLE);

		if (after_location)
		{
			opts_iterator	modif_it = _modifiers.find(token.getContent());
			if (modif_it != _modifiers.end())
				return ParserToken::fromLexerToken(token, modif_it->second);
		}

		opts_iterator	direct_it = _directive_keywords.find(token.getContent());
		if (direct_it != _directive_keywords.end())
			return ParserToken::fromLexerToken(token, direct_it->second);

		opts_iterator	block_it = _block_keywords.find(token.getContent());
		if (block_it != _block_keywords.end())
			return ParserToken::fromLexerToken(token, block_it->second);

		if (token == LexerTokenType::END) return ParserToken::fromLexerToken(token, ParserTokenType::PT_END);

		return ParserToken::fromLexerToken(token, ParserTokenType::PT_WORD);
	}

	ParserNode	*parseStatement()
	{
		_skipComments();

		ParserToken	name = _convertToken(*_it);
		if (name == PT_WORD)
			_errors.push_back((*_it).getLineAddress() + " Invalid node name!");
		++_it;

		bool	is_after_location = name == ParserTokenType::PT_LOCATION;
		std::vector<ParserToken>	values;
		while (_it
			&& *_it != LexerTokenType::COMMENT
			&& *_it != LexerTokenType::LBRACE
			&& *_it != LexerTokenType::SEMICOLON)
		{
			if (*_it == LexerTokenType::RBRACE)
				_errors.push_back((*_it).getLineAddress() + " Closing an unopened scope!");
			values.push_back(_convertToken(*_it, is_after_location));
			is_after_location = false;
			++_it;
		}
		if (*_it == LexerTokenType::LBRACE)
		{
			LexerIterator::token	lbrace = *_it;
			++_it; // consome { LBRACE
			Block	*block = new Block(name, values);
			while (_it && *_it != LexerTokenType::RBRACE)
			{
				_skipComments();
				if (!_it || *_it == LexerTokenType::LBRACE)
					break;
				block->children.push_back(parseStatement());
			}
			if (*_it != LexerTokenType::RBRACE)
				_errors.push_back(lbrace.getLineAddress() + " Unclosed scope");
			++_it; // consome } RBRACE
			return block;
		}
		++_it; // consome ; SEMICOLON
		if (name == PT_SERVER)
			name.setType(PT_SERVER_DIRECTIVE);
		Directive	*directive = new Directive(name, values);
		return directive;
	}

public:
	Parser(LexerBuilder &lexer_builder): _lexer_builder(lexer_builder), _it(), _block_keywords(), _directive_keywords(), _modifiers() {};
	Parser(): _lexer_builder(), _it(), _block_keywords(), _directive_keywords(), _modifiers() {};
	~Parser() {};

	void	addBlockKeyword(const std::string &content, ParserTokenType type) { _block_keywords[content] = type; }
	void	addDirectiveKeyword(const std::string &content, ParserTokenType type) { _directive_keywords[content] = type; }
	void	addModifier(const std::string &content, ParserTokenType type) { _modifiers[content] = type; }
	void	setLexer(LexerBuilder &lexer_builder) { _lexer_builder = lexer_builder; }

	ParserAst	parse()
	{
		_it = _lexer_builder.build();
		ParserAst	ast;
		if (!_it.error().empty())
			ast.addError(_it.error());
		Block	*root = new Block(ParserToken("root", 0, 0, "root", ParserTokenType::PT_MAIN));
		while (_it)
		{
			_skipComments();
			if (!_it)
				break;
			root->children.push_back(parseStatement());
		}
		for (size_t i = 0; i < _errors.size(); i++)
			ast.addError(_errors[i]);
		ast.setRoot(root);
		return ast;
	}
};

#endif
