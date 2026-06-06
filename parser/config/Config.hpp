/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 14:54:54 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/06 15:03:44 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <vector>

# include "singleton.hpp"
# include "Server.hpp"

class Config: public patterns::singleton<Config>
{
	Config() {};
	~Config() {};
	Config(const Config &);
	Config	&operator=(const Config &);

	friend class patterns::singleton<Config>;

	private:
		std::vector<Server>	_servers;

	public:
};

#endif
