/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserVisitor.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 15:42:20 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/05/31 15:45:07 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_VISITOR_HPP
# define PARSER_VISITOR_HPP

struct Directive;
struct Block;

struct ParserVisitor
{
	virtual ~ParserVisitor() {};
	virtual void	visit(Directive&) = 0;
	virtual void	visit(Block&) = 0;
};

#endif
