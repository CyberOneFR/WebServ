#pragma once

#include <iostream>
#include <vector>
#include "ServerConfig.hpp"

class	WebServConfig
{
	private:
		struct ParsingInfo
		{
			std::ifstream	stream;
			std::string		path;
			size_t			line_number;
			size_t			column_number;
		};
		std::vector<ServerConfig>	_servers;

		void	handle_word(ParsingInfo &info, std::string &word);
	public:
		~WebServConfig();
		WebServConfig(const std::string &path);
		WebServConfig(const WebServConfig &copy);
		WebServConfig	&operator=(const WebServConfig &other);

	class WebServSyntaxException: public std::exception
	{
		private:
			std::string	_message;
		public:
			virtual ~WebServSyntaxException() throw();
			WebServSyntaxException(ParsingInfo &info, const std::string &message);
			WebServSyntaxException(const WebServSyntaxException &copy);
			WebServSyntaxException	&operator=(const WebServSyntaxException &other);

			const char	*what(void) const throw();
	};
};