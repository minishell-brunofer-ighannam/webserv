/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserVisitorValidator.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 17:28:35 by brunofer          #+#    #+#             */
/*   Updated: 2026/06/04 14:28:55 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_VISITOR_VALIDATOR_HPP
# define PARSER_VISITOR_VALIDATOR_HPP

# include "ScopeValidator.hpp"
# include "ArgAmountValidator.hpp"
# include "ParserVisitorBase.hpp"
# include "ParserComposite.hpp"

class ParserVisitorValidator: public ParserVisitorBase
{
private:
	ScopeValidator				_scope_validator;
	ArgAmountValidator			_arg_amount_validator;
	std::vector<std::string>	_errors;

public:
	ParserVisitorValidator(): _scope_validator(ScopeValidatorBuilder::defaultScopes()), _arg_amount_validator(ArgAmountValidatorBuilder::defaultValidations()) {};
	ParserVisitorValidator(
		ScopeValidatorBuilder &scope_validator_builder,
		ArgAmountValidatorBuilder &arg_amount_validator_builder)
			: _scope_validator(scope_validator_builder.build()),
			_arg_amount_validator(arg_amount_validator_builder.build()) {};
	~ParserVisitorValidator() {};

	void	visit(Directive &directive)
	{
		if (directive.name == PT_END || directive.name == PT_WORD) return;
		_arg_amount_validator.validate(directive);
		_arg_amount_validator.dumpErrorsOn(_errors);
	};

	void	visit(Block &block)
	{
		if (block.name == PT_END || block.name == PT_WORD) return;
		if (block.name != PT_MAIN)
			_arg_amount_validator.validate(block);
		_arg_amount_validator.dumpErrorsOn(_errors);

		_scope_validator.pushScope(block.name.getType());
		_scope_validator.validate(block);
		_scope_validator.dumpErrorsOn(_errors);
		for (size_t i = 0; i < block.children.size(); i++)
			block.children[i]->accept(*this);
		_scope_validator.popScope();
	};

	const std::vector<std::string> &getErrors() const { return _errors; }
};

#endif
