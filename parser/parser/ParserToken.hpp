/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserToken.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 11:02:08 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/05/31 18:34:59 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_TOKEN_HPP
# define PARSER_TOKEN_HPP

# include <string>

# include "has_type.hpp"
# include "LexerToken.hpp"
# include "ParserTokenType.hpp"

class ParserToken: public has_type<ParserTokenType>
{
	typedef has_type<ParserTokenType>	base;
private:
	std::string	_file_name;
	size_t		_line;
	size_t		_line_col;
	std::string	_content;
public:
	ParserToken(const std::string &file_name, size_t line, size_t line_col, const std::string &content, ParserTokenType type)
		: base(type), _file_name(file_name), _line(line), _line_col(line_col), _content(content) {};
	ParserToken(const std::string &file_name, size_t line, size_t line_col, const std::string &content)
		: _file_name(file_name), _line(line), _line_col(line_col), _content(content) {};
	~ParserToken() {};

	static ParserToken	fromLexerToken(const LexerToken &token, ParserTokenType type)
	{
		return ParserToken(token.getFileName(), token.getLine(), token.getLineColumn(), token.getContent(), type);
	}

	const std::string	&getContent() const { return _content; }
	const std::string	&getFileName() const { return _file_name; }
	size_t				getLine() const { return _line; }
	size_t				getLineColumn() const { return _line_col; }

	ParserToken	&operator=(ParserTokenType type) { _type = type; return *this; }
	bool		operator==(ParserTokenType type) const { return _type == type; }
	bool		operator!=(ParserTokenType type) const { return !(*this == type); }
};

#endif
