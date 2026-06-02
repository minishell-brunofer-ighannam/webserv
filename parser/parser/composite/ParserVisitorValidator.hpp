/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserVisitorValidator.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunofer <brunofer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 17:28:35 by brunofer          #+#    #+#             */
/*   Updated: 2026/06/02 18:39:09 by brunofer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_VISITOR_VALIDATOR_HPP
# define PARSER_VISITOR_VALIDATOR_HPP

# include <map>
# include <vector>
# include <algorithm>

# include "ParserVisitor.hpp"
# include "ParserTokenType.hpp"
# include "ParserComposite.hpp"

class ParserVisitorValidator: public ParserVisitor
{
private:
	std::vector<ParserTokenType>	_curr_scopes;
	typedef std::map<ParserTokenType, std::vector<ParserTokenType>> ScopeValidator;
	std::map<ParserTokenType, std::vector<ParserTokenType>>	_directive_scope;
	std::map<ParserTokenType, std::vector<ParserTokenType>>	_block_scope;
	std::vector<std::string>	_errors;

	static std::string	getStringTypes(std::vector<ParserTokenType> scopes)
	{
		std::string resp;
		for (size_t i = 0; i < scopes.size(); i++)
		{
			resp += ParserTokenTypeStr[scopes[i]];
			resp += " ";
		}
		return resp;
	}

	void	_validateScopeFrom(ParserToken	&name, ScopeValidator &validator)
	{
		ScopeValidator::iterator validator_it = validator.find(name.getType());
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
			std::string msg = name.getContent() + " in wrong context. Must be in scopes '" + getStringTypes(scopes) + "'";
			_errors.push_back(msg);
		}
	}

public:
	ParserVisitorValidator(): _directive_scope(), _block_scope() {};
	~ParserVisitorValidator() {};


	void	visit(Directive &directive)
	{
		_validateScopeFrom(directive.name, _directive_scope);
	};

	void	visit(Block &block)
	{
		_validateScopeFrom(block.name, _block_scope);
		_curr_scopes.push_back(block.name.getType());
	};

	void addDirectiveScopeValidation(ParserTokenType directive, std::vector<ParserTokenType> scopes)
	{
		_directive_scope[directive] = scopes;
	}

	void addBlockScopeValidation(ParserTokenType block, std::vector<ParserTokenType> scopes)
	{
		_block_scope[block] = scopes;
	}
};

#endif
