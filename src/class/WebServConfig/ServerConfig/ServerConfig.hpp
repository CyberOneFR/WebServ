#pragma once

#include <fstream>
#include <vector>
#include <map>
#include "LocationConfig.hpp"
#include "WebServConfig.hpp"

class	ServerConfig
{
	enum ServerSyntaxState
	{
		DEFAULT,
		COMMENT,
		STRING_SIMPLE,
		STRING_DOUBLE,
		ESCAPE,
		LISTEN,
		HOST,
		SERVER_NAME,
		ERROR_PAGE,
		CLIENT_MAX_BODY_SIZE,
		ROOT,
		INDEX,
		LOCATION,
	};
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
		ServerConfig(ParsingInfo &info);
		ServerConfig(const ServerConfig &copy);
		ServerConfig	&operator=(const ServerConfig &other);
};