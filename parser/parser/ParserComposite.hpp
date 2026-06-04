/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserComposite.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 15:26:33 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/03 19:52:51 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_COMPOSITE_HPP
# define PARSER_COMPOSITE_HPP

# include <vector>

# include "ParserVisitorBase.hpp"
# include "ParserToken.hpp"

enum ParserNodeType
{
	DIRECTIVE,
	BLOCK
};

struct ParserNode
{
	ParserToken					name;
	std::vector<ParserToken>	values;
	ParserNodeType				node_type;
	virtual void	accept(ParserVisitorBase&) = 0;
	ParserNode(const ParserToken &_name, ParserNodeType type): name(_name), values(), node_type(type) {}
	ParserNode(const ParserToken &_name, const std::vector<ParserToken> &_values, ParserNodeType type): name(_name), values(_values), node_type(type) {}
	virtual ~ParserNode() {};
};

struct Directive: public ParserNode
{
	Directive(const ParserToken &_name): ParserNode(_name, DIRECTIVE) {}
	Directive(const ParserToken &_name, const std::vector<ParserToken> &_values): ParserNode(_name, _values, DIRECTIVE) {}
	void	accept(ParserVisitorBase &visitor) { visitor.visit(*this); }
};

struct Block: public ParserNode
{
	std::vector<ParserNode*>	children;
	Block(const ParserToken &_name): ParserNode(_name, BLOCK), children() {}
	Block(const ParserToken &_name, const std::vector<ParserToken> &_values): ParserNode(_name, _values, BLOCK) {}
	void	accept(ParserVisitorBase &visitor) { visitor.visit(*this); }
};


#endif
