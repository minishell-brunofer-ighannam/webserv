/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserAst.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 18:46:01 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/04 12:50:58 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_AST_HPP
# define PARSER_AST_HPP

# include "ParserComposite.hpp"
# include "ParserVisitorPrinter.hpp"
# include "ParserVisitorValidator.hpp"

class ParserAst
{
private:
	Block	*_root;
	std::vector<std::string>	_errors;
public:
	ParserAst(Block *root): _root(root) {};
	ParserAst(): _root(NULL) {};
	~ParserAst() {};

	void	print() const
	{
		if (!_root) return;
		ParserVisitorPrinter	printer;
		_root->accept(printer);
	}

	void	validateStructure()
	{
		if (!_root) return;
		ParserVisitorValidator	validator;
		_root->accept(validator);
		const std::vector<std::string>	&errors = validator.getErrors();
		for (size_t i = 0; i < errors.size(); i++)
			addError(errors[i]);
	}

	bool	hasError() const { return !_errors.empty(); }
	void	printErrors() const
	{
		for (size_t i = 0; i < _errors.size(); i++)
			std::cout << _errors[i] << "\n";
	}

	void	setRoot(Block *root)
	{
		if (_root)
			delete _root;
		_root = root;
	}

	void	addError(const std::string &error) { _errors.push_back(error); }
};

#endif
