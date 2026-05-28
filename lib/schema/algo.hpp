/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunofer <brunofer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 19:11:32 by brunofer          #+#    #+#             */
/*   Updated: 2026/05/28 20:47:28 by brunofer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	ALGO_HPP
# define ALGO_HPP

#include <string>
#include <vector>

struct error
{
	std::string title;
	std::string msg;
};

template <typename T>
struct result
{
	bool success;
	T value;
	std::vector<error> errors;
	result(T _value) : success(true), value(_value), errors() {};
	result(std::vector<error> _errors) : success(false), value(), errors(_errors) {};
	static result success(T value) { return result(value); };
	static result failure(std::vector<error> _errors) { return result(_errors); };
};



template <typename T, typename Derived>
class schema_base
{
	public:
		struct functor_validator
		{
			virtual std::string operator()(T value) = 0;
		}
		struct functor_refine
		{
			virtual bool operator()(T value, std::string msg) = 0;
		}
	private:
		std::string field_name;
		std::vector<functor_validator*> validators;
	public:
		Derived& name(std::string _name) { field_name = _name; return static_cast<Derived&>(*this); };
		Derived& refine(functor_refine* refine_validator, std::string msg)
		{
			struct ref: public functor_validator
			{
				std::string _msg;
				ref(std::string& message) : _msg(message) {};
				std::string operator()(T value)
				{
					bool item = refine_validator(value, _msg);
					delete refine_validator;
					return item ? "" : _msg;
				}
			};
			addValidator(new ref(msg));
			return static_cast<Derived&>(*this);
		}
		void addValidator(functor_validator* validator)
		{
			if (!validator)
				return;
			validators.push_back(validator);
		};


};




#endif
