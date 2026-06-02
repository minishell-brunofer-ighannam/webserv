/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserVisitor.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunofer <brunofer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 15:42:20 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/02 14:18:24 by brunofer         ###   ########.fr       */
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
