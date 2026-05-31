/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 09:50:53 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/05/31 16:44:49 by bruno-valer      ###   ########.fr       */
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
# include "LexerToken.hpp"


std::ostream	&operator<<(std::ostream &os, LexerToken token)
{
	size_t		size = token.getContent().size();
	bool		has_endl = token.getContent()[size - 1] == '\n';
	std::string	content = has_endl ? token.getContent().substr(0, size - 1) : token.getContent();
	os << "<" << token.getType() << ">[" << content << "]" << (has_endl ? "\n" : "");
	return os;
}

class Lexer
{
	public:
		typedef LexerTokenType					token_type;
		typedef LexerToken						token;
	private:
	typedef	std::pair<std::string, token_type>	keyword;
	std::vector<keyword>		_delimitters;
	std::vector<std::string>	_comments;
	std::vector<std::string>	_quotes;
	File						_file;
	std::string					_error;
	std::vector<token>			_tokens;

	keyword						_temp_delimitter;
	std::string					_temp_quote;

	public:
		Lexer(const std::string &file_name)
			: _delimitters(), _comments(), _quotes(), _file(file_name), _error(), _temp_delimitter(), _temp_quote() {};
		Lexer()
			: _delimitters(), _comments(), _quotes(), _file(), _error(), _temp_delimitter(), _temp_quote() {};
		~Lexer() {};

	private:
		void	_skipWhiteSpaces()
		{
			while (_file && std::isspace(*_file))
				++_file;
		}

		bool	_isDelimitter()
		{
			for (size_t kw = 0; kw < _delimitters.size(); kw++)
			{
				if (_file.nextIs(_delimitters[kw].first))
				{
					_temp_delimitter = _delimitters[kw];
					return true;
				}
			}
			return false;
		}

		token	_readKeyword()
		{
			_file += _temp_delimitter.first.size();
			return token(_file.name(), _file.line(), _file.lineColumn(), _temp_delimitter.first, _temp_delimitter.second);
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
						token	_token = token::fromQuote(_file.name(), _file.line(), _file.lineColumn(), _temp_quote, _file.substr(quote_start_cursor, _file.cursor() - quote_start_cursor));
						_file += _temp_quote.size();
						return _token;
					}
				}
				if (*_file == '\\')
					++_file;
				++_file;
			}
			std::string e_header = _file.name()+":"+std::to_string(quote_start_line)+":"+std::to_string(quote_start_line_col)+" -> ";
			_error = e_header + "quote open '"+_temp_quote+"'.";
			return token(_file.name(), _file.line(), _file.lineColumn(), "", token_type::ERROR);
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
				++_file;
			return	token(_file.name(), _file.line(), _file.lineColumn(), _file.substr_back(comment_start_cursor), token_type::COMMENT);
		}

		token	_readWord()
		{
			if (!_file)
				return token(_file.name(), _file.line(), _file.lineColumn(), "", token_type::END);
			size_t	word_start_cursor		= _file.cursor();
			while (*_file && !std::isspace(*_file) && !_isDelimitter() && !_isQuote())
				++_file;
			return token(_file.name(), _file.line(), _file.lineColumn(), _file.substr(word_start_cursor, _file.cursor() - word_start_cursor), token_type::WORD);
		}

	public:
		void	setFile(const std::string &file_name) { _file.setName(file_name); }
		void	addDelimitter(const std::string &_keyword, token_type _type) { _delimitters.push_back(std::make_pair(_keyword, _type)); }
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
				if (_isDelimitter())
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
