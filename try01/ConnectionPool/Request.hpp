/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 02:13:29 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/10 02:29:22 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>

# include "SocketConnection.hpp"

struct RequestMethod
{
	enum type
	{
		GET,
		POST,
		PUT,
		PATCH,
		DELETE
	};
};

class Request
{
	private:
		SocketConnection	*_connection;

	public:
		typedef RequestMethod::type Method;
		Method		method;
		std::string	body;

		Request(SocketConnection *conn): _connection(conn), method(), body() {}

};

#endif
