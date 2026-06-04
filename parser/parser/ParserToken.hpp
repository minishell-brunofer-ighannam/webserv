/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserToken.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 11:02:08 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/04 14:14:57 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_TOKEN_HPP
# define PARSER_TOKEN_HPP

# include <string>

# include "has_type.hpp"
# include "LexerToken.hpp"
# include "ParserTokenType.hpp"

class ParserToken: public BaseToken<ParserTokenType>
{
	typedef BaseToken<ParserTokenType>	base;
public:
	ParserToken(const std::string &file_name, size_t line, size_t line_col, const std::string &content, ParserTokenType type)
		: base(file_name, line, line_col, content, type) {};
	ParserToken(const std::string &file_name, size_t line, size_t line_col, const std::string &content)
		: base(file_name, line, line_col, content) {};
	~ParserToken() {};

	static ParserToken	fromLexerToken(const LexerToken &token, ParserTokenType type)
	{
		return ParserToken(token.getFileName(), token.getLine(), token.getLineColumn(), token.getContent(), type);
	}
	void				setType(ParserTokenType type) { _type = type; }

	ParserToken	&operator=(ParserTokenType type) { _type = type; return *this; }
};

#endif
