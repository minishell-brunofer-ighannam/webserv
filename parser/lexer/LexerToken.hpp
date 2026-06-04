/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LexerToken.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 11:44:53 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/04 03:09:21 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_TOKEN_HPP
# define LEXER_TOKEN_HPP

# include <iostream>

# include "has_type.hpp"
# include "LexerTokenType.hpp"

class LexerToken: public segregation::has_type<LexerTokenType>
{
	typedef segregation::has_type<LexerTokenType>	base;
	public:
		typedef LexerTokenType		type;
	private:
		std::string	_file_name;
		size_t		_line;
		size_t		_line_col;
		std::string	_content;

	public:
		LexerToken(const std::string &file_name, size_t line, size_t line_col, const std::string &content, type _type)
			: base(_type), _file_name(file_name), _line(line), _line_col(line_col), _content(content)  {};
		~LexerToken() {};

		static LexerToken	fromQuote(const std::string &file_name, size_t line, size_t line_col, const std::string &quote, const std::string &content)
		{
			size_t	_type = 0;
			size_t	size = quote.size() - 1;
			for (size_t i = 0; i < quote.size(); i++)
			{
				_type |= quote[i] << (size * 8);
				size--;
			}
			return LexerToken(file_name, line, line_col, content, static_cast<LexerTokenType>(_type));
		}
		const std::string	&getFileName() const { return _file_name; }
		size_t				getLine() const { return _line; }
		size_t				getLineColumn() const { return _line_col; }
		std::string			getLineAddress() const
		{
			return _file_name + ":" + std::to_string(_line) + ":" + std::to_string(_line_col);
		}
		const std::string	&getContent() const { return _content; }

		bool	operator==(LexerTokenType type) const { return this->_type == type; }
		bool	operator!=(LexerTokenType type) const { return !(*this == type); }
};

#endif
