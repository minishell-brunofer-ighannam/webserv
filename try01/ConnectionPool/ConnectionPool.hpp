/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionPool.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 01:54:10 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/10 02:57:07 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_POOL_HPP
# define CONNECTION_POOL_HPP

# include <map>
# include <vector>

# include "singleton.hpp"
# include "IMultiplexer.hpp"
# include "Server.hpp"
# include "RequestBuilder.hpp"

/**
 * ConnectionPool sera:
 *
 * - um `singleton`;
 * - um `OberverSubject` e os `Servers` serão os `Observers` dele;
 */
class ConnectionPool: public patterns::singleton<ConnectionPool>
{
	friend class patterns::singleton<ConnectionPool>;
private:
	IMultiplexer				*_multiplexer;
	std::map<Socket*, Server*>	_listenners;
	std::vector<RequestBuilder>	_pending_request;

public:
	ConnectionPool(IMultiplexer *multiplexer): _multiplexer(multiplexer) { }
	~ConnectionPool() {}

	void	addListenner(Socket *socket, Server *server)
	{
		_multiplexer->add(socket);
		_listenners[socket] = server;
	}

	void	waitConnections()
	{
		while (true)
		{
			SocketEventList	events;
			_multiplexer->wait(events);
			for (size_t i = 0; i < events.size(); i++)
			{
				/* code */
			}
		}
	}
};

#endif
