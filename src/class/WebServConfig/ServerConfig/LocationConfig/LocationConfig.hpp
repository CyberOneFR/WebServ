#pragma once

#include <fstream>
#include <vector>
#include <map>

class	LocationConfig
{
	private:
		std::string					_path;
		std::vector<std::string>	_methods;
		std::string					_root;
		std::string					_index;
		bool						_autoindex;
		std::string					_redirections;
		std::vector<LocationConfig>	_locations;
	public:
		~LocationConfig();
		LocationConfig(std::ifstream &stream);
		LocationConfig(const LocationConfig &copy);
		LocationConfig	&operator=(const LocationConfig &other);
		bool	match(const std::string &request_path) const;
};