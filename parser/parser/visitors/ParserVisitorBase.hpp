/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserVisitorBase.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 15:42:20 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/03 12:01:25 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_VISITOR_BASE_HPP
# define PARSER_VISITOR_BASE_HPP

struct Directive;
struct Block;

struct ParserVisitorBase
{
	virtual ~ParserVisitorBase() {};
	virtual void	visit(Directive&) = 0;
	virtual void	visit(Block&) = 0;
};

#endif
