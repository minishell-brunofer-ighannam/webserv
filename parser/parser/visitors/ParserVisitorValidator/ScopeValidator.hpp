/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScopeValidator.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 16:47:55 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/04 02:19:25 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOPE_VALIDATOR_HPP
# define SCOPE_VALIDATOR_HPP

# include <map>
# include <vector>
# include <algorithm>

# include "ParserTokenType.hpp"
# include "ParserComposite.hpp"

class ScopeValidator
{
	private:
		std::vector<ParserTokenType>	_curr_scopes;
		typedef std::map<ParserTokenType, std::vector<ParserTokenType>> Validations;
		std::map<ParserTokenType, std::vector<ParserTokenType>>	_directive_scope;
		std::map<ParserTokenType, std::vector<ParserTokenType>>	_block_scope;
		std::vector<std::string>	_errors;

		static std::string	_getStringTypes(std::vector<ParserTokenType> scopes)
		{
			std::string resp;
			for (size_t i = 0; i < scopes.size(); i++)
			{
				resp += ParserTokenTypeStr[scopes[i]];
				resp += " ";
			}
			return resp;
		}

		void	_validateScopeFrom(ParserToken	&name, Validations &validator)
		{
			Validations::iterator validator_it = validator.find(name.getType());
			if (validator_it == validator.end())
			{
				std::string msg = name.getContent() + " scope not found.";
				_errors.push_back(msg);
				return;
			}
			std::vector<ParserTokenType> &scopes = validator_it->second;
			std::vector<ParserTokenType>::iterator scope_it = std::find(scopes.begin(), scopes.end(), _curr_scopes[_curr_scopes.size() - 1]);
			if (scope_it == scopes.end())
			{
				std::string msg = name.getContent() + " in wrong context. Must be in scopes '" + _getStringTypes(scopes) + "'";
				_errors.push_back(msg);
			}
		}

	public:
		ScopeValidator(): _curr_scopes(), _directive_scope(), _block_scope() {};
		~ScopeValidator() {};

		void	validate(Block &block)
		{
			for (size_t i = 0; i < block.children.size(); i++)
			{
				if (block.children[i]->node_type == BLOCK)
					_validateScopeFrom(block.children[i]->name, _block_scope);
				else
					_validateScopeFrom(block.children[i]->name, _directive_scope);
			}

		}

		void	pushScope(ParserTokenType scope)	{ _curr_scopes.push_back(scope); }
		void	popScope()							{ _curr_scopes.pop_back(); }

		void addDirectiveScopeValidation(ParserTokenType directive, ParserTokenType scope)
		{
			_directive_scope[directive].push_back(scope);
		}

		void addBlockScopeValidation(ParserTokenType block, ParserTokenType scope)
		{
			_block_scope[block].push_back(scope);
		}

		void	dumpErrorsOn(std::vector<std::string> &destine)
		{
			for (size_t i = 0; i < _errors.size(); i++)
				destine.push_back(_errors[i]);
			_errors.clear();
		}
};

class ScopeValidatorBuilder
{
	private:
		ScopeValidator	_scope_validator;
		ParserTokenType	_last_scope_updated;
		bool			_last_scope_updated_is_block;
		ParserTokenType	_tracking_scope;
	public:
		ScopeValidatorBuilder()
			: _scope_validator(), _last_scope_updated(), _last_scope_updated_is_block(false), _tracking_scope() {};
		~ScopeValidatorBuilder() {};

		static ScopeValidator	defaultScopes()
		{
			ScopeValidatorBuilder	builder;
			builder.withDefaultBlockScopes();
			builder.withDefaultDirectiveScopes();
			return builder.build();
		}

		ScopeValidator	build() { return _scope_validator; }

		ScopeValidatorBuilder	&withDefaultDirectiveScopes()
		{
			return withDirectiveOnScope(PT_WORKER_PROCESSES, PT_MAIN)
				.withDirectiveOnScope(PT_WORKER_CONNECTIONS, PT_EVENTS)
				.withDirectiveOnScope(PT_LISTEN, PT_SERVER)
				.withDirectiveOnScope(PT_SERVER_NAME, PT_SERVER)
				.withDirectiveOnScope(PT_SSL_CERTIFICATE, PT_SERVER)
				.withDirectiveOnScope(PT_SSL_CERTIFICATE_KEY, PT_SERVER)
				.withDirectiveOnScope(PT_SSL_PROTOCOLS, PT_SERVER)
				.withDirectiveOnScope(PT_PROXY_PASS, PT_LOCATION)
				.withDirectiveOnScope(PT_FASTCGI_PASS, PT_LOCATION)
				.withDirectiveOnScope(PT_EXPIRES, PT_LOCATION)
				.withDirectiveOnScope(PT_ROOT, PT_LOCATION).andOn(PT_HTTP).andOn(PT_SERVER)
				.withDirectiveOnScope(PT_INDEX, PT_LOCATION).andOn(PT_HTTP).andOn(PT_SERVER)
				.withDirectiveOnScope(PT_ACCESS_LOG, PT_LOCATION).andOn(PT_HTTP).andOn(PT_SERVER)
				.withDirectiveOnScope(PT_ERROR_PAGE, PT_LOCATION).andOn(PT_HTTP).andOn(PT_SERVER)
				.withDirectiveOnScope(PT_ADD_HEADER, PT_LOCATION).andOn(PT_HTTP).andOn(PT_SERVER)
				.withDirectiveOnScope(PT_TRY_FILES, PT_LOCATION).andOn(PT_SERVER);
		}

		ScopeValidatorBuilder	&withDefaultBlockScopes()
		{
			return withBlockOnScope(PT_EVENTS, PT_MAIN)
				.withBlockOnScope(PT_HTTP, PT_MAIN)
				.withBlockOnScope(PT_UPSTREAM, PT_HTTP)
				.withBlockOnScope(PT_SERVER, PT_HTTP)
				.withBlockOnScope(PT_LOCATION, PT_SERVER).andOn(PT_LOCATION);
		}

		ScopeValidatorBuilder	&withDirectiveOnScope(ParserTokenType directive, ParserTokenType scope)
		{
			_last_scope_updated = directive;
			_last_scope_updated_is_block = false;
			_scope_validator.addDirectiveScopeValidation(directive, scope);
			return *this;
		}

		ScopeValidatorBuilder	&withBlockOnScope(ParserTokenType block, ParserTokenType scope)
		{
			_last_scope_updated = block;
			_last_scope_updated_is_block = true;
			_scope_validator.addBlockScopeValidation(block, scope);
			return *this;
		}

		ScopeValidatorBuilder	&andOn(ParserTokenType scope)
		{
			if (_last_scope_updated_is_block)
				_scope_validator.addBlockScopeValidation(_last_scope_updated, scope);
			else
				_scope_validator.addDirectiveScopeValidation(_last_scope_updated, scope);
			return *this;
		}
};

#endif
