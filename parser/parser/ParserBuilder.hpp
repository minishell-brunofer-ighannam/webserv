/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserBuilder.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 17:40:15 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/04 13:13:14 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PARSER_BUILDER_HPP
# define PARSER_BUILDER_HPP

# include "Parser.hpp"

class ParserBuilder
{
private:
	Parser			_parser;
	LexerBuilder	_lexer_builder;
public:
	ParserBuilder(): _parser() {};
	~ParserBuilder() {};

	static	ParserBuilder	defaultBuilder()
	{
		return ParserBuilder()
			.withDefaultBlocks().withDefaultDirectives()
			.withDefaultLexer().withDefaultModifiers();
	}

	ParserAst			build() { return _parser.parse(); }

	ParserBuilder	&withDefaultLexer()
	{
		_lexer_builder = LexerBuilder().withDefaultTokens();
		_parser.setLexer(_lexer_builder);
		return *this;
	}

	ParserBuilder	&withDefaultDirectives()
	{
		_parser.addDirectiveKeyword("worker_processes", ParserTokenType::PT_WORKER_PROCESSES);
		_parser.addDirectiveKeyword("error_log", ParserTokenType::PT_ERROR_LOG);
		_parser.addDirectiveKeyword("pid", ParserTokenType::PT_PID);
		_parser.addDirectiveKeyword("use", ParserTokenType::PT_USE);
		_parser.addDirectiveKeyword("multi_accept", ParserTokenType::MULTI_ACCEPT);
		_parser.addDirectiveKeyword("default_type", ParserTokenType::DEFAULT_TYPE);
		_parser.addDirectiveKeyword("sendfile", ParserTokenType::SENDFILE);
		_parser.addDirectiveKeyword("keepalive_timeout", ParserTokenType::KEEPALIVE_TIMEOUT);
		_parser.addDirectiveKeyword("log_format", ParserTokenType::PT_LOG_FORMAT);
		_parser.addDirectiveKeyword("autoindex", ParserTokenType::PT_AUTOINDEX);
		_parser.addDirectiveKeyword("client_max_body_size", ParserTokenType::PT_CLIENT_MAX_BODY_SIZE);
		_parser.addDirectiveKeyword("log_not_found", ParserTokenType::PT_LOG_NOT_FOUND);
		_parser.addDirectiveKeyword("proxy_set_header", ParserTokenType::PT_PROXY_SET_HEADER);
		_parser.addDirectiveKeyword("proxy_cache_bypass", ParserTokenType::PT_PROXY_CACHE_BYPASS);
		_parser.addDirectiveKeyword("fastcgi_index", ParserTokenType::PT_FASTCGI_INDEX);
		_parser.addDirectiveKeyword("fastcgi_param", ParserTokenType::PT_FASTCGI_PARAM);
		_parser.addDirectiveKeyword("worker_connections", ParserTokenType::PT_WORKER_CONNECTIONS);
		_parser.addDirectiveKeyword("expires", ParserTokenType::PT_EXPIRES);
		_parser.addDirectiveKeyword("access_log", ParserTokenType::PT_ACCESS_LOG);
		_parser.addDirectiveKeyword("error_page", ParserTokenType::PT_ERROR_PAGE);
		_parser.addDirectiveKeyword("try_files", ParserTokenType::PT_TRY_FILES);
		_parser.addDirectiveKeyword("add_header", ParserTokenType::PT_ADD_HEADER);
		_parser.addDirectiveKeyword("listen", ParserTokenType::PT_LISTEN);
		_parser.addDirectiveKeyword("server_name", ParserTokenType::PT_SERVER_NAME);
		_parser.addDirectiveKeyword("root", ParserTokenType::PT_ROOT);
		_parser.addDirectiveKeyword("index", ParserTokenType::PT_INDEX);
		_parser.addDirectiveKeyword("include", ParserTokenType::PT_INCLUDE);
		_parser.addDirectiveKeyword("proxy_pass", ParserTokenType::PT_PROXY_PASS);
		_parser.addDirectiveKeyword("fastcgi_pass", ParserTokenType::PT_FASTCGI_PASS);
		_parser.addDirectiveKeyword("return", ParserTokenType::PT_RETURN);
		_parser.addDirectiveKeyword("rewrite", ParserTokenType::PT_REWRITE);
		_parser.addDirectiveKeyword("ssl_certificate", ParserTokenType::PT_SSL_CERTIFICATE);
		_parser.addDirectiveKeyword("ssl_certificate_key", ParserTokenType::PT_SSL_CERTIFICATE_KEY);
		_parser.addDirectiveKeyword("ssl_protocols", ParserTokenType::PT_SSL_PROTOCOLS);
		_parser.addDirectiveKeyword("ssl_ciphers", ParserTokenType::PT_SSL_CIPHERS);
		return *this;
	}

	ParserBuilder	&withDefaultBlocks()
	{
		_parser.addBlockKeyword("http", ParserTokenType::PT_HTTP);
		_parser.addBlockKeyword("events", ParserTokenType::PT_EVENTS);
		_parser.addBlockKeyword("server", ParserTokenType::PT_SERVER);
		_parser.addBlockKeyword("location", ParserTokenType::PT_LOCATION);
		_parser.addBlockKeyword("upstream", ParserTokenType::PT_UPSTREAM);
		_parser.addBlockKeyword("geo", ParserTokenType::PT_GEO);
		_parser.addBlockKeyword("map", ParserTokenType::PT_MAP);
		_parser.addBlockKeyword("stream", ParserTokenType::PT_STREAM);
		_parser.addBlockKeyword("types", ParserTokenType::PT_TYPES);
		_parser.addBlockKeyword("limit_except", ParserTokenType::PT_LIMIT_EXCEPT);
		return *this;
	}

	ParserBuilder	&withDefaultModifiers()
	{
		_parser.addModifier("=", ParserTokenType::PT_MOD_EXACT);
		_parser.addModifier("~", ParserTokenType::PT_MOD_REGEX);
		_parser.addModifier("~*", ParserTokenType::PT_MOD_REGEX_CI);
		_parser.addModifier("^~", ParserTokenType::PT_MOD_PREFIX);
		return *this;
	}

	ParserBuilder	&withDirective(const std::string &content, ParserTokenType type)
	{
		_parser.addDirectiveKeyword(content, type);
		return *this;
	}
	ParserBuilder	&withBlock(const std::string &content, ParserTokenType type)
	{
		_parser.addBlockKeyword(content, type);
		return *this;
	}
	ParserBuilder	&withModifier(const std::string &content, ParserTokenType type)
	{
		_parser.addModifier(content, type);
		return *this;
	}

	ParserBuilder	&withFile(const std::string &file_name)
	{
		_lexer_builder.withFile(file_name);
		_parser.setLexer(_lexer_builder);
		return *this;
	}
};

#endif
