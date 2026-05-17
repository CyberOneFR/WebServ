#include "WebServ.hpp"

WebServ::~WebServ()
{
}

WebServ::WebServ(const std::string  &config_path)try: _config(config_path)
{
}
catch (std::exception &e)
{
}

WebServ::WebServ(const WebServ &copy): _config(copy._config)
{
}

WebServ	&WebServ::operator=(const WebServ &other)
{
	if (this != &other)
	{
		_config = other._config;
	}
	return *this;
}

void	WebServ::start(void)
{
}