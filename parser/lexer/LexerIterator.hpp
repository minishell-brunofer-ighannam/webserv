/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LexerIterator.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 22:33:46 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/04 03:01:59 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_ITERATOR_HPP
# define LEXER_ITERATOR_HPP

# include "Lexer.hpp"

class LexerIterator
{
	public:
	typedef Lexer::token	token;
private:
	std::vector<token>	_tokens;
	size_t				_idx;
	std::string			_error;
public:
	LexerIterator(const Lexer &lexer): _tokens(lexer.tokens()), _idx(0), _error(lexer.error()) {};
	LexerIterator(): _tokens(), _idx(0), _error() {};
	~LexerIterator() {};


	size_t		size() const { return _tokens.size(); }
	std::string	error() { return _error; }

	void	show() const
	{
		for (size_t i = 0; i < _tokens.size(); i++)
			std::cout << _tokens[i] << " ";
	}

	LexerIterator	&operator=(const Lexer &lexer) { _tokens = lexer.tokens(); _error = lexer.error(); return *this; }
	operator bool() { return _idx < _tokens.size(); }

	const token	&operator->() const
	{
		if (_idx == size()) return _tokens[size() - 1];
		return _tokens[_idx];
	}

	const token	&operator*() const
	{
		if (_idx == size()) return _tokens[size() - 1];
		return _tokens[_idx];
	}

	LexerIterator	&operator++()
	{
		if (_idx < size())
			_idx++;
		return *this;
	}

	LexerIterator	operator++(int)
	{
		LexerIterator	copy(*this);
		++(*this);
		return copy;
	}

	LexerIterator	&operator+=(size_t value)
	{
		while (value > 0 && _idx < size())
		{
			++(*this);
			--value;
		}
		return *this;
	}

	LexerIterator	&operator--()
	{
		if (_idx > 0)
			_idx--;
		return *this;
	}

	LexerIterator	operator--(int)
	{
		LexerIterator	copy(*this);
		--(*this);
		return copy;
	}

	LexerIterator	&operator-=(size_t value)
	{
		while (value > 0 && _idx > 0)
		{
			--(*this);
			--value;
		}
		return *this;
	}
};

#endif
