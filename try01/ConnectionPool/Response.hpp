/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 02:14:21 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/10 02:30:09 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "SocketConnection.hpp"

class Response
{
	private:
		SocketConnection	*_connection;

	public:
		Response(SocketConnection *conn): _connection(conn) {}
		~Response() {}
};

#endif
