/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 09:50:53 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/05/31 00:59:13 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
# define LEXER_HPP

# include <string>
# include <vector>
# include <set>
# include <iostream>
# include <sstream>

# include "has_type.hpp"
# include "File.hpp"

template <typename LexerTokenType>
class LexerToken: public has_type<LexerTokenType>
{
	typedef has_type<LexerTokenType>	base;
	public:
		typedef LexerTokenType		type;
	private:
		std::string	_content;

	public:
		LexerToken(const std::string &content, type _type): base(_type), _content(content)  {};
		~LexerToken() {};
		static LexerToken	fromQuote(const std::string &quote, const std::string &content)
		{
			size_t	_type = 0;
			size_t	size = quote.size() - 1;
			for (size_t i = 0; i < quote.size(); i++)
			{
				_type |= quote[i] << (size * 8);
				size--;
			}
			LexerToken	_token(content, static_cast<LexerTokenType>(_type));
			return _token;
		}
		const std::string	&getContent() const { return _content; }
};

template <typename LexerTokenType>
std::ostream	&operator<<(std::ostream &os, LexerToken<LexerTokenType> token)
{
	size_t		size = token.getContent().size();
	bool		has_endl = token.getContent()[size - 1] == '\n';
	std::string	content = has_endl ? token.getContent().substr(0, size - 1) : token.getContent();
	os << "<" << token.getType() << ">[" << content << "]" << (has_endl ? "\n" : "");
	return os;
}

template <typename LexerTokenType>
class Lexer
{
	public:
		typedef LexerTokenType					token_type;
		typedef LexerToken<token_type>			token;
	private:
	typedef	std::pair<std::string, token_type>	keyword;
	std::vector<keyword>		_keywords;
	std::vector<std::string>	_comments;
	std::vector<std::string>	_quotes;
	File						_file;
	std::string					_error;
	std::vector<token>			_tokens;

	keyword						_temp_keyword;
	std::string					_temp_quote;

	public:
		Lexer(const std::string &file_name)
			: _keywords(), _comments(), _quotes(), _file(file_name), _error(), _temp_keyword(), _temp_quote() {};
		Lexer()
			: _keywords(), _comments(), _quotes(), _file(), _error(), _temp_keyword(), _temp_quote() {};
		~Lexer() {};

	private:
		void	_skipWhiteSpaces()
		{
			while (_file && std::isspace(*_file))
				_file++;
		}

		bool	_isKeyword()
		{
			for (size_t kw = 0; kw < _keywords.size(); kw++)
			{
				if (_file.nextIs(_keywords[kw].first))
				{
					_temp_keyword = _keywords[kw];
					return true;
				}
			}
			return false;
		}

		token	_readKeyword()
		{
			_file += _temp_keyword.first.size();
			return token(_temp_keyword.first, _temp_keyword.second);
		}

		bool	_isQuote()
		{
			for (size_t qt = 0; qt < _quotes.size(); qt++)
			{
				if (_file.nextIs(_quotes[qt]))
				{
					_temp_quote = _quotes[qt];
					return true;
				}
			}
			return false;
		}

		token	_readQuote()
		{
			_file += _temp_quote.size();
			size_t	quote_start_cursor		= _file.cursor();
			size_t	quote_start_line		= _file.line();
			size_t	quote_start_line_col	= _file.lineColumn();
			while (_file)
			{
				if (*_file == _temp_quote[0])
				{
					if (_file.nextIs(_temp_quote))
					{
						token	_token = token::fromQuote(_temp_quote, _file.substr(quote_start_cursor, _file.cursor() - quote_start_cursor));
						_file += _temp_quote.size();
						return _token;
					}
				}
				if (*_file == '\\')
					_file++;
				_file++;
			}
			std::string e_header = _file.name()+":"+std::to_string(quote_start_line)+":"+std::to_string(quote_start_line_col)+" -> ";
			_error = e_header + "quote open '"+_temp_quote+"'.";
			return token("", token_type::ERROR);
		}

		bool	_isComment()
		{
			for (size_t qt = 0; qt < _comments.size(); qt++)
			{
				if (_file.nextIs(_comments[qt]))
					return true;
			}
			return false;
		}

		token	_readComment()
		{
			size_t	comment_start_cursor		= _file.cursor();
			while (*_file && *_file != '\n')
				_file++;
			return	token(_file.substr_back(comment_start_cursor), token_type::COMMENT);
		}

		token	_readWord()
		{
			if (!_file)
				return token("", token_type::END);
			size_t	word_start_cursor		= _file.cursor();
			while (*_file && !std::isspace(*_file) && !_isKeyword() && !_isQuote())
				_file++;
			return token(_file.substr(word_start_cursor, _file.cursor() - word_start_cursor), token_type::WORD);
		}

	public:
		void	setFile(const std::string &file_name) { _file.setName(file_name); }
		void	addKeyword(const std::string &_keyword, token_type _type) { _keywords.push_back(std::make_pair(_keyword, _type)); }
		void	addComment(const std::string &comment) { _comments.push_back(comment); }
		void	addQuote(const std::string &quote) { _quotes.push_back(quote); }
		void	run()
		{
			_file.run();
			if (!_file.error().empty())
			{
				_error = _file.error();
				return;
			}
			while (_file)
			{
				_skipWhiteSpaces();
				if (_isComment())
				{
					_tokens.push_back(_readComment());
					continue;
				}
				if (_isKeyword())
				{
					_tokens.push_back(_readKeyword());
					continue;
				}
				if (_isQuote())
				{
					_tokens.push_back(_readQuote());
					continue;
				}
				_tokens.push_back(_readWord());
			}
		}


		const std::string			&error() const { return _error; }

		const std::vector<token>	&tokens() const { return _tokens; }

		void	show() const
		{
			for (size_t i = 0; i < _tokens.size(); i++)
				std::cout << _tokens[i] << " ";
		}
};

#endif
