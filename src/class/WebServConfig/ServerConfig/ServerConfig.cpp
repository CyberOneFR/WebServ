#include "ServerConfig.hpp"
#include <fstream>

ServerConfig::~ServerConfig()
{
}

ServerConfig::ServerConfig(std::ifstream &stream)
{
	(void)stream;
}

ServerConfig::ServerConfig(const ServerConfig &copy)
{
	(void)copy;
}

ServerConfig	&ServerConfig::operator=(const ServerConfig &other)
{
	(void)other;
	return *this;
}