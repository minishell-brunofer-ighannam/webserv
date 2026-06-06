/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 14:52:37 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/06 14:54:38 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>

# include "ServerListen.hpp"

class Server
{
private:
	std::vector<ServerListen>	_listeners;

public:
	Server() {};
	~Server() {};
};

#endif
