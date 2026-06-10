/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBuilder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 02:31:28 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/10 03:51:12 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_BUILDER_HPP
# define REQUEST_BUILDER_HPP

# include <string>

# include "Request.hpp"
# include "SocketConnection.hpp"

class RequestBuilder
{
private:
	Request		_req;
	std::string	_buffer;
	bool		_is_headers_read;
	bool		_is_complete;

public:
	RequestBuilder(SocketConnection *conn)
		: _req(conn), _buffer(), _is_complete(false) {}
	~RequestBuilder() {}

	Request	build() const { return _req; }

	void	addToBuffer(const std::string buff)
	{
		_buffer += buff;
		size_t	idx = _buffer.find("\r\n\r\n");
		if (idx != std::string::npos)
			processHeader(idx - 1);
	}

	void	processHeader(size_t last)
	{

	}

	bool	isComplete() const { return _is_complete; }
};

#endif
