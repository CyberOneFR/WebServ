#pragma once

#include <string>
#include "WebServConfig.hpp"

class	WebServ
{
	private:
		WebServConfig	_config;
	public:
		~WebServ();
		WebServ(const std::string &config_path);
		WebServ(const WebServ &copy);
		WebServ	&operator=(const WebServ &other);

		void	start(void);
};