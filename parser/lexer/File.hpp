/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunofer <brunofer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 19:50:46 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/02 15:29:13 by brunofer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
# define FILE_HPP

# include <string>
# include <fstream>
# include <limits>
# include <vector>
# include <iostream>

class File
{
private:
	std::string					_name;
	std::string					_error;
	std::string					_src;
	std::vector<std::string>	_lines;
	size_t						_idx;
	size_t						_line;
	size_t						_line_col;

	void	_readFile()
	{
		std::ifstream file(_name.c_str());
		if (!file.is_open())
		{
			_error = "Cannot open file '" + _name + "'.";
			return;
		}
		std::string	line;
		while (std::getline(file, line))
		{
			_lines.push_back(line);
			_src += line;
			_src += '\n';
		}
	}

public:
	File(std::string name)
		: _name(name), _error(), _src(), _idx(0), _line(1), _line_col(0) { _readFile(); };
	File()
		: _name(), _error(), _src(), _idx(0), _line(1), _line_col(0) {};
	~File() {};

	void				run()
	{
		if (_src.empty())
			_readFile();
	}
	void				setName(const std::string &file_name) { _name = file_name; _readFile(); }
	const std::string	&name() const { return _name; }
	size_t				line() const { return _line; }
	size_t				lineColumn() const { return _line_col; }
	size_t				cursor() const { return _idx; }
	size_t				size() const { return _src.size(); }
	bool				hasError() { return !_error.empty(); }
	const std::string	&error() const { return _error; }

	bool				nextIs(const std::string &str)
	{
		if (str.size() + _idx > size())
			return false;
		for (size_t i = 0; i < str.size(); i++)
			if (_src[_idx + i] != str[i])
				return false;
		return true;
	}

	std::string			substr_forward(size_t size = std::numeric_limits<size_t>::max())
	{
		return _src.substr(_idx, size);
	}

	std::string			substr_back(size_t pos)
	{
		if (pos >= _idx) return "";
		return _src.substr(pos, _idx - pos + 1);
	}

	std::string			substr(size_t pos, size_t size = std::numeric_limits<size_t>::max())
	{
		if (pos >= _idx) return "";
		return _src.substr(pos, size);
	}

	const std::string	*lineContentAt(size_t idx) const
	{
		if (idx >= _lines.size()) return NULL;
		return &_lines[idx];
	}

	const std::string	*lineContent() const
	{
		if (_line >= _lines.size()) return NULL;
		return &_lines[_line];
	}

	operator			bool() { return _idx < _src.size(); }

	File				&operator+=(size_t value)
	{
		while (value > 0 && _idx < _src.size())
		{
			++(*this);
			value--;
		}
		return *this;
	}

	File				operator++(int)
	{
		File	copy(*this);
		if (_src[_idx] == '\n')
		{
			++(*this);
			return copy;
		}
		++(*this);
		return copy;
	}

	File				&operator++()
	{
		if (_idx == _src.size()) return *this;
		if (_src[_idx] == '\n')
		{
			_line_col = 0;
			_line++;
			_idx++;
			return *this;
		}
		_idx++;
		_line_col++;
		return *this;
	}

	File				&operator-=(size_t value)
	{
		if (value > _idx)
			value = _idx + 1;
		while (value > 0)
		{
			--(*this);
			--value;
		}
		return *this;
	}

	File				operator--(int)
	{
		File	copy(*this);
		if (_src[_idx] == '\n')
		{
			--(*this);
			return copy;
		}
		--(*this);
		return copy;
	}

	File				&operator--()
	{
		if (_idx == 0) return *this;
		_idx--;
		if (_src[_idx] == '\n')
		{
			_line--;
			_line_col = _lines[_line].size();
			return *this;
		}
		_line_col--;
		return *this;
	}

	char				&operator*()
	{
		return _src[_idx];
	}

	const char			&operator*() const
	{
		return _src[_idx];
	}

	char				&operator->()
	{
		return _src[_idx];
	}

	const char			&operator->() const
	{
		return _src[_idx];
	}
};

#endif
