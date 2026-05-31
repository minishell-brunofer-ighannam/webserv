/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserTokenType.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 11:52:51 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/05/31 16:14:01 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_TOKEN_TYPE_HPP
# define PARSER_TOKEN_TYPE_HPP

# include <iostream>

enum ParserTokenType
{
	// tipos léxicos
	PT_COMMENT,
	PT_LBRACE,
	PT_RBRACE,
	PT_SEMICOLON,
	PT_STRING_SINGLE,
	PT_STRING_DOUBLE,
	PT_WORD,
	PT_ERROR,
	PT_END,

	// blocos
	PT_HTTP,
	PT_EVENTS,
	PT_SERVER,
	PT_LOCATION,
	PT_UPSTREAM,
	PT_GEO,
	PT_MAP,
	PT_STREAM,
	PT_TYPES,
	PT_LIMIT_EXCEPT,

	// diretivas especiais
	PT_LISTEN,
	PT_SERVER_NAME,
	PT_ROOT,
	PT_INDEX,
	PT_INCLUDE,
	PT_PROXY_PASS,
	PT_FASTCGI_PASS,
	PT_RETURN,
	PT_REWRITE,
	PT_SSL_CERTIFICATE,
	PT_SSL_CERTIFICATE_KEY,
	PT_SSL_PROTOCOLS,

	// modificadores de location
	PT_MOD_EXACT,        // =
	PT_MOD_REGEX,        // ~
	PT_MOD_REGEX_CI,     // ~*
	PT_MOD_PREFIX,       // ^~

	// valores semânticos
	PT_BOOL_ON,
	PT_BOOL_OFF,
};

std::ostream	&operator<<(std::ostream &os, ParserTokenType type)
{
	switch (type)
	{
	case ParserTokenType::PT_COMMENT :
		os << "PT_COMMENT";
		break;
	case ParserTokenType::PT_LBRACE :
		os << "PT_LBRACE";
		break;
	case ParserTokenType::PT_RBRACE :
		os << "PT_RBRACE";
		break;
	case ParserTokenType::PT_SEMICOLON :
		os << "PT_SEMICOLON";
		break;
	case ParserTokenType::PT_STRING_SINGLE :
		os << "PT_STRING_SINGLE";
		break;
	case ParserTokenType::PT_STRING_DOUBLE :
		os << "PT_STRING_DOUBLE";
		break;
	case ParserTokenType::PT_WORD :
		os << "PT_WORD";
		break;
	case ParserTokenType::PT_ERROR :
		os << "PT_ERROR";
		break;
	case ParserTokenType::PT_END :
		os << "PT_END";
		break;
	case ParserTokenType::PT_HTTP :
		os << "PT_HTTP";
		break;
	case ParserTokenType::PT_EVENTS :
		os << "PT_EVENTS";
		break;
	case ParserTokenType::PT_SERVER :
		os << "PT_SERVER";
		break;
	case ParserTokenType::PT_LOCATION :
		os << "PT_LOCATION";
		break;
	case ParserTokenType::PT_UPSTREAM :
		os << "PT_UPSTREAM";
		break;
	case ParserTokenType::PT_GEO :
		os << "PT_GEO";
		break;
	case ParserTokenType::PT_MAP :
		os << "PT_MAP";
		break;
	case ParserTokenType::PT_STREAM :
		os << "PT_STREAM";
		break;
	case ParserTokenType::PT_TYPES :
		os << "PT_TYPES";
		break;
	case ParserTokenType::PT_LIMIT_EXCEPT :
		os << "PT_LIMIT_EXCEPT";
		break;
	case ParserTokenType::PT_LISTEN :
		os << "PT_LISTEN";
		break;
	case ParserTokenType::PT_SERVER_NAME :
		os << "PT_SERVER_NAME";
		break;
	case ParserTokenType::PT_ROOT :
		os << "PT_ROOT";
		break;
	case ParserTokenType::PT_INDEX :
		os << "PT_INDEX";
		break;
	case ParserTokenType::PT_INCLUDE :
		os << "PT_INCLUDE";
		break;
	case ParserTokenType::PT_PROXY_PASS :
		os << "PT_PROXY_PASS";
		break;
	case ParserTokenType::PT_FASTCGI_PASS :
		os << "PT_FASTCGI_PASS";
		break;
	case ParserTokenType::PT_RETURN :
		os << "PT_RETURN";
		break;
	case ParserTokenType::PT_REWRITE :
		os << "PT_REWRITE";
		break;
	case ParserTokenType::PT_SSL_CERTIFICATE :
		os << "PT_SSL_CERTIFICATE";
		break;
	case ParserTokenType::PT_SSL_CERTIFICATE_KEY :
		os << "PT_SSL_CERTIFICATE_KEY";
		break;
	case ParserTokenType::PT_SSL_PROTOCOLS :
		os << "PT_SSL_PROTOCOLS";
		break;
	case ParserTokenType::PT_MOD_EXACT :
		os << "PT_MOD_EXACT";
		break;
	case ParserTokenType::PT_MOD_REGEX :
		os << "PT_MOD_REGEX";
		break;
	case ParserTokenType::PT_MOD_REGEX_CI :
		os << "PT_MOD_REGEX_CI";
		break;
	case ParserTokenType::PT_MOD_PREFIX :
		os << "PT_MOD_PREFIX";
		break;
	case ParserTokenType::PT_BOOL_ON :
		os << "PT_BOOL_ON";
		break;
	case ParserTokenType::PT_BOOL_OFF :
		os << "PT_BOOL_OFF";
		break;

	default:
		break;
	}
	return os;
}

#endif
