#pragma once

#include <fstream>
#include <vector>
#include <map>

class	LocationConfig
{
	enum LocationSyntaxState
	{
		DEFAULT,
		COMMENT,
		STRING_SIMPLE,
		STRING_DOUBLE,
		ESCAPE
	};
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
		LocationConfig(ParsingInfo &info);
		LocationConfig(const LocationConfig &copy);
		LocationConfig	&operator=(const LocationConfig &other);

		static void StateDefault(ParsingInfo &info, std::stack<LocationSyntaxState> &state_stack, std::string &word, char c);
};