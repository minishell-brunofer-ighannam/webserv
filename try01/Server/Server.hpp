/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 02:08:12 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/10 02:17:54 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Request.hpp"
# include "Response.hpp"

class Server
{
private:


public:
	Server() {}
	~Server() {}

	void	handleRequest(const Request &req, Response &res)
	{
		(void)req;
		(void)res;
	}
};

#endif
