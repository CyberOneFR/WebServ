#pragma once

#include <iostream>
#include <vector>
#include "ServerConfig.hpp"

struct ParsingInfo
{
	std::ifstream	stream;
	std::string		path;
	size_t			line_number;
	size_t			column_number;
};

class	WebServConfig
{
	private:
		enum WebServSyntaxState
		{
			DEFAULT,
			WORD,
			COMMENT,
			STRING_SIMPLE,
			STRING_DOUBLE,
			ESCAPE,
			SERVER,
		};
		std::vector<ServerConfig>	_servers;

		void	handle_directive(ParsingInfo &info, std::stack<WebServSyntaxState> &state_stack, std::string &word, char c);
	public:
		~WebServConfig();
		WebServConfig(const std::string &path);
		WebServConfig(const WebServConfig &copy);
		WebServConfig	&operator=(const WebServConfig &other);
	class SyntaxException: public std::exception
	{
		private:
			std::string	_message;
		public:
			virtual ~SyntaxException() throw();
			SyntaxException(ParsingInfo &info, const std::string &message);
			SyntaxException(const SyntaxException &copy);
			SyntaxException	&operator=(const SyntaxException &other);

			const char	*what(void) const throw();
	};
};