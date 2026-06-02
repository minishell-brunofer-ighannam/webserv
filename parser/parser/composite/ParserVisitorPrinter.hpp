/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserVisitorPrinter.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunofer <brunofer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 18:28:35 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/02 15:28:28 by brunofer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_VISITOR_PRINTER_HPP
# define PARSER_VISITOR_PRINTER_HPP

# include <string>
# include <iostream>
# include <sstream>

# include "ParserVisitor.hpp"
# include "ParserComposite.hpp"

class ParserVisitorPrinter: public ParserVisitor
{
private:
	size_t	_ident;

	std::string	getPrefix() const { return std::string(4 * _ident, ' '); }
public:
	ParserVisitorPrinter(): _ident(0) {};
	~ParserVisitorPrinter() {};

	void	visit(Directive &directive)
	{
		if (directive.name == PT_END) return;
		std::stringstream	ss;
		ss << getPrefix() << directive.name.getContent() << " -> ";
		for (size_t i = 0; i < directive.values.size(); i++)
		{
			if (directive.values[i] == PT_END) return;
			ss << directive.values[i].getContent() << " ";
		}
		ss << "\n";
		std::cout << ss.str();
	}

	void	visit(Block &block)
	{
		std::cout << getPrefix() << "[" << block.name.getContent() << "] ";
		for (size_t i = 0; i < block.values.size(); i++)
			std::cout << block.values[i].getContent() << " ";
		std::cout << "\n";
		++_ident;
		for (size_t i = 0; i < block._children.size(); i++)
			block._children[i]->accept(*this);
		--_ident;
	}
};

#endif
