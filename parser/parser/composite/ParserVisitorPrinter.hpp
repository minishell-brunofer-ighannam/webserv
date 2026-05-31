/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserVisitorPrinter.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 18:28:35 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/05/31 19:10:32 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_VISITOR_PRINTER_HPP
# define PARSER_VISITOR_PRINTER_HPP

# include <string>
# include <iostream>

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
		std::cout << getPrefix() << directive.name.getContent() << " -> ";
		for (size_t i = 0; i < directive.values.size(); i++)
			std::cout << directive.values[i].getContent() << " ";
		std::cout << "\n";
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
