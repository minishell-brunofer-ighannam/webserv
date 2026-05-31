/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserComposite.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 15:26:33 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/05/31 17:06:39 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_COMPOSITE_HPP
# define PARSER_COMPOSITE_HPP

# include <vector>

# include "ParserVisitor.hpp"
# include "ParserToken.hpp"

struct ParserNode
{
	ParserToken					name;
	std::vector<ParserToken>	values;
	virtual void	accept(ParserVisitor&) = 0;
	ParserNode(const ParserToken &_name): name(_name), values() {}
	ParserNode(const ParserToken &_name, const std::vector<ParserToken> &_values): name(_name), values(_values) {}
	virtual ~ParserNode() {};
};

struct Directive: public ParserNode
{
	Directive(const ParserToken &_name): ParserNode(_name) {}
	Directive(const ParserToken &_name, const std::vector<ParserToken> &_values): ParserNode(_name, _values) {}
	void	accept(ParserVisitor &visitor) { visitor.visit(*this); }
};

struct Block: public ParserNode
{
	std::vector<ParserNode*>	_children;
	Block(const ParserToken &_name): ParserNode(_name), _children() {}
	Block(const ParserToken &_name, const std::vector<ParserToken> &_values): ParserNode(_name, _values) {}
	void	accept(ParserVisitor &visitor) { visitor.visit(*this); }
};


#endif
