/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_type.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 10:13:58 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/05/30 10:22:25 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HAS_TYPE_HPP
# define HAS_TYPE_HPP

template <typename T>
class has_type
{
	private:
		T	_type;

	protected:
		has_type(): _type() {};
		void	setType(T type) { _type = type; }
	public:
		has_type(T type): _type(type) {};
		~has_type() {};

		T	getType() const { return _type; }
};

#endif
