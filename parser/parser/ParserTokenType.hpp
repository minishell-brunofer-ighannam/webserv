/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserTokenType.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunofer <brunofer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 11:52:51 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/02 17:48:36 by brunofer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_TOKEN_TYPE_HPP
# define PARSER_TOKEN_TYPE_HPP

# include <iostream>

enum ParserTokenType
{
	PT_MAIN,

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
	PT_WORKER_PROCESSES,
	PT_WORKER_CONNECTIONS,
	PT_LISTEN,
	PT_SERVER_NAME,
	PT_EXPIRES,
	PT_ROOT,
	PT_ACCESS_LOG,
	PT_ERROR_PAGE,
	PT_TRY_FILES,
	PT_ADD_HEADER,
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

static const char* ParserTokenTypeStr[] =
{
	"PT_MAIN",

	// tipos léxicos
	"PT_COMMENT",
	"PT_LBRACE",
	"PT_RBRACE",
	"PT_SEMICOLON",
	"PT_STRING_SINGLE",
	"PT_STRING_DOUBLE",
	"PT_WORD",
	"PT_ERROR",
	"PT_END",

	// blocos
	"PT_HTTP",
	"PT_EVENTS",
	"PT_SERVER",
	"PT_LOCATION",
	"PT_UPSTREAM",
	"PT_GEO",
	"PT_MAP",
	"PT_STREAM",
	"PT_TYPES",
	"PT_LIMIT_EXCEPT",

	// diretivas especiais
	"PT_WORKER_PROCESSES",
	"PT_WORKER_CONNECTIONS",
	"PT_LISTEN",
	"PT_SERVER_NAME",
	"PT_EXPIRES",
	"PT_ROOT",
	"PT_ACCESS_LOG",
	"PT_ERROR_PAGE",
	"PT_TRY_FILES",
	"PT_ADD_HEADER",
	"PT_INDEX",
	"PT_INCLUDE",
	"PT_PROXY_PASS",
	"PT_FASTCGI_PASS",
	"PT_RETURN",
	"PT_REWRITE",
	"PT_SSL_CERTIFICATE",
	"PT_SSL_CERTIFICATE_KEY",
	"PT_SSL_PROTOCOLS",

	// modificadores de location
	"PT_MOD_EXACT",        // =
	"PT_MOD_REGEX",        // ~
	"PT_MOD_REGEX_CI",     // ~*
	"PT_MOD_PREFIX",       // ^~

	// valores semânticos
	"PT_BOOL_ON",
	"PT_BOOL_OFF",
};

std::ostream	&operator<<(std::ostream &os, ParserTokenType type)
{
	os << ParserTokenTypeStr[type];
	return os;
}

#endif
