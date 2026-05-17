#include "LocationConfig.hpp"
#include <fstream>

LocationConfig::~LocationConfig()
{
}

LocationConfig::LocationConfig(std::ifstream &stream)
{
	(void)stream;
}

LocationConfig::LocationConfig(const LocationConfig &copy)
{
	(void)copy;
}

LocationConfig	&LocationConfig::operator=(const LocationConfig &other)
{
	(void)other;
	return *this;
}