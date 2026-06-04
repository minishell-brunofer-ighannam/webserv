/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArgAmountValidator.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 19:59:52 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/04 02:19:29 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARG_AMOUNT_VALIDATOR_HPP
# define ARG_AMOUNT_VALIDATOR_HPP

# include <map>
# include <vector>
# include <string>
# include <limits>

# include "schema.hpp"
# include "ParserTokenType.hpp"
# include "ParserComposite.hpp"

class ArgAmountValidator
{
private:
	typedef std::map<ParserTokenType, schema::detail::integer_base<unsigned> > Validations;
	std::map<ParserTokenType, schema::detail::integer_base<unsigned> >	_validations;
	std::vector<std::string>	_errors;
public:
	ArgAmountValidator() {};
	~ArgAmountValidator() {};

	void	validate(const ParserNode &node)
	{
		Validations::iterator	it = _validations.find(node.name.getType());
		if (it == _validations.end())
		{
			std::string msg = "Type" + std::string(ParserTokenTypeStr[node.name.getType()]);
			msg += " not found on validations.";
			_errors.push_back(msg);
			return;
		}
		schema::result<unsigned int> resp = it->second.parse(std::to_string(node.values.size()));
		if (!resp)
			for (size_t i = 0; i < resp.errors.size(); i++)
				_errors.push_back(resp.errors[i].format());
	}

	void	addValidation(ParserTokenType type, schema::detail::integer_base<unsigned> validation)
	{
		_validations[type] = validation;
	}

	void	dumpErrorsOn(std::vector<std::string> &destine)
	{
		for (size_t i = 0; i < _errors.size(); i++)
			destine.push_back(_errors[i]);
		_errors.clear();
	}
};

class ArgAmountValidatorBuilder
{
private:
	ArgAmountValidator	_validator;
public:
	ArgAmountValidatorBuilder() {};
	~ArgAmountValidatorBuilder() {};

	static ArgAmountValidator	defaultValidations()
	{
		ArgAmountValidatorBuilder builder;
		return builder.withDefaultValidations().build();
	}

	ArgAmountValidator			build() { return _validator; }

	ArgAmountValidatorBuilder	&withDefaultValidations()
	{
		static unsigned int	unlimited = std::numeric_limits<unsigned>::max();
		return add(PT_HTTP, schema::uint32().between(0, 0).name("http args amount"))
		.add(PT_EVENTS, schema::uint32().between(0, 0).name("events args amount"))
		.add(PT_SERVER, schema::uint32().between(0, 0).name("server args amount"))
		.add(PT_UPSTREAM, schema::uint32().between(1, 1).name("upstream args amount"))
		.add(PT_LOCATION, schema::uint32().between(1, 2).name("location args amount"))
		.add(PT_WORKER_PROCESSES, schema::uint32().between(1, 1).name("worket_process args amount"))
		.add(PT_WORKER_CONNECTIONS, schema::uint32().between(1, 1).name("worker_connections args amount"))
		.add(PT_ERROR_LOG, schema::uint32().between(1, 2).name("error_log args amount"))
		.add(PT_PID, schema::uint32().between(1, 1).name("pid args amount"))
		.add(PT_LISTEN, schema::uint32().between(1, unlimited).name("listen args amount"))
		.add(PT_SERVER_NAME, schema::uint32().between(1, unlimited).name("server_name args amount"))
		.add(PT_ROOT, schema::uint32().between(1, 1).name("root args amount"))
		.add(PT_INDEX, schema::uint32().between(1, unlimited).name("index args amount"))
		.add(PT_TRY_FILES, schema::uint32().between(2, unlimited).name("try_files args amount"))
		.add(PT_PROXY_PASS, schema::uint32().between(1, 1).name("index args amount"))
		.add(PT_PROXY_SET_HEADER, schema::uint32().between(2, 2).name("proxy_set_header args amount"))
		.add(PT_FASTCGI_PASS, schema::uint32().between(1, 1).name("fast_cgi_pass args amount"))
		.add(PT_FASTCGI_PARAM, schema::uint32().between(2, 2).name("fast_cgi_param args amount"))
		.add(PT_ERROR_PAGE, schema::uint32().between(2, unlimited).name("error_page args amount"))
		.add(PT_ADD_HEADER, schema::uint32().between(2, 3).name("add_header args amount"))
		.add(PT_EXPIRES, schema::uint32().between(1, 1).name("expires args amount"))
		.add(PT_SSL_CERTIFICATE, schema::uint32().between(1, 1).name("ssl_certificate args amount"))
		.add(PT_SSL_CERTIFICATE_KEY, schema::uint32().between(1, 1).name("ssl_certificate_key args amount"))
		.add(PT_SSL_PROTOCOLS, schema::uint32().between(1, unlimited).name("ssl_protocols args amount"))
		.add(PT_SSL_CIPHERS, schema::uint32().between(1, 1).name("ssl_ciphers args amount"))
		.add(PT_CLIENT_MAX_BODY_SIZE, schema::uint32().between(1, 1).name("client_max_body_size args amount"))
		.add(PT_LOG_FORMAT, schema::uint32().between(2, unlimited).name("log_format args amount"))
		.add(PT_ACCESS_LOG, schema::uint32().between(1, 2).name("access_log args amount"))
		.add(PT_RETURN, schema::uint32().between(1, 2).name("return args amount"));
	}

	ArgAmountValidatorBuilder	&add(ParserTokenType type, schema::detail::integer_base<unsigned> validation)
	{
		_validator.addValidation(type, validation);
		return *this;
	}
};

#endif
