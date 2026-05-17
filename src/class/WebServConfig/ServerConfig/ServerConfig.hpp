#pragma once

#include <fstream>
#include <vector>
#include <map>
#include "LocationConfig.hpp"

class	ServerConfig
{
	private:
		int							_port;
		std::string					_host;
		std::vector<std::string>	_server_names;
		std::map<int, std::string>	_error_pages;
		size_t						_client_max_body_size;
		std::string					_root;
		std::string					_index;
		std::vector<LocationConfig>	_locations;
	public:
		~ServerConfig();
		ServerConfig(std::ifstream &stream);
		ServerConfig(const ServerConfig &copy);
		ServerConfig	&operator=(const ServerConfig &other);
};