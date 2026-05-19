#include "ServerConfig.hpp"
#include "WebServConfig.hpp"
#include <fstream>
#include <iostream>
#include <stack>
#include <netdb.h>
#include <cstring>

ServerConfig::~ServerConfig()
{
}

ServerConfig::ServerConfig(ParsingInfo &info): _port(-1), _host(""), _client_max_body_size(0), _root(""), _index("")
{
	std::stack<ServerSyntaxState>	state_stack;
	std::string						word;
	char							c;

	state_stack.push(DEFAULT);
	while (info.stream.get(c))
	{
		if (c == '\n')
		{
			info.line_number++;
			info.column_number = 0;
		}
		else
			info.column_number++;
		switch (state_stack.top())
		{
			case COMMENT:
			{
				if (c == '\n')
					state_stack.pop();
				else if (!std::isblank(c) && !std::isgraph(c))
					throw WebServConfig::SyntaxException(info, "Unexpected control character");
				break;
			}
			case STRING_SIMPLE:
			{
				if (!std::isblank(c) && !std::isgraph(c))
					throw WebServConfig::SyntaxException(info, "Unexpected control character");
				else if (c == '\'')
					state_stack.pop();
				else if (c == '\\')
					state_stack.push(ESCAPE);
				else
					word += c;
				break;
			}
			case STRING_DOUBLE:
			{
				if (!std::isblank(c) && !std::isgraph(c))
					throw WebServConfig::SyntaxException(info, "Unexpected control character");
				else if (c == '"')
					state_stack.pop();
				else if (c == '\\')
					state_stack.push(ESCAPE);
				else
					word += c;
				break;
			}
			case ESCAPE:
			{
				word += c;
				state_stack.pop();
				break;
			}
			case DEFAULT:
			{
				if (word.compare("listen") == 0)
					state_stack.push(LISTEN);
				else if (word.compare("host") == 0)
					state_stack.push(HOST);
				else if (word.compare("server_name") == 0)
					state_stack.push(SERVER_NAME);
				else if (word.compare("error_page") == 0)
					state_stack.push(ERROR_PAGE);
				else if (word.compare("client_max_body_size") == 0)
					state_stack.push(CLIENT_MAX_BODY_SIZE);
				else if (word.compare("root") == 0)
					state_stack.push(ROOT);
				else if (word.compare("index") == 0)
					state_stack.push(INDEX);
				else if (word.compare("location") == 0)
					state_stack.push(LOCATION);
				else if (std::isspace(c))
				{
					if (!word.empty())
						continue;
					else
						throw WebServConfig::SyntaxException(info, "Unknown directive: " + word);
				}
				if (std::iscntrl(c))
					throw WebServConfig::SyntaxException(info, "Unexpected control character");
				else if (c == '{' || c == ';')
					throw WebServConfig::SyntaxException(info, "Unexpected '" + std::string(1, c) + "'");
				else if (c == '}')
				{
					if (this->_port == -1 || this->_root.empty())
						throw WebServConfig::SyntaxException(info, "Unexpected '}' missing required directives (need at least 'listen' and 'root')");
					else
						return;
				}
				else if (c == '#')
					state_stack.push(COMMENT);
				else if (c == '\'')
					state_stack.push(STRING_SIMPLE);
				else if (c == '"')
					state_stack.push(STRING_DOUBLE);
				else if (c == '\\')
					state_stack.push(ESCAPE);
				else
					word += c;
				break;
			}
			case LISTEN:
			{
				if (std::isblank(c) || c == ';')
				{
					if (c == ';' && word.empty() && this->_port == -1)
						throw WebServConfig::SyntaxException(info, "Unexpected '" + std::string(1, c) + "' missing port number");
					else if (!word.empty())
					{
						if (word.find_first_not_of("0123456789") != std::string::npos)
							throw WebServConfig::SyntaxException(info, "Invalid port number: " + word + " (must be a number)");
						try
						{
							this->_port = std::stol(word);
						}
						catch (const std::exception &e)
						{
							throw WebServConfig::SyntaxException(info, "Invalid port number: " + word + " (" + e.what() + ")");
						}
						if (this->_port < 0 || this->_port > 65535)
							throw WebServConfig::SyntaxException(info, "Invalid port number: " + word + " (must be between 0 and 65535)");
						state_stack.pop();
						word.clear();
					}
					else
						continue;
				}
				else if (c == '\'')
					state_stack.push(STRING_SIMPLE);
				else if (c == '"')
					state_stack.push(STRING_DOUBLE);
				else if (c == '\\')
					state_stack.push(ESCAPE);
				else
					word += c;
				break;
			}
			case HOST:
			{
				if (std::isblank(c) || c == ';')
				{
					if (c == ';' && word.empty() && this->_host.empty())
						throw WebServConfig::SyntaxException(info, "Unexpected '" + std::string(1, c) + "' missing host address/name");
					else if (!word.empty())
					{
						struct addrinfo	*res = nullptr;
						struct addrinfo	hints;

						std::memset(&hints, 0, sizeof(hints));
    					hints.ai_family = AF_INET;
    					hints.ai_socktype = SOCK_STREAM;
						int	status = getaddrinfo(word.c_str(), nullptr, &hints, &res);
						if (status != 0)
							throw WebServConfig::SyntaxException(info, "Error resolving " + word + " : " + std::string(gai_strerror(status)));
						this->_host = word;
						std::cout << "Set host: " << word << std::endl;
						freeaddrinfo(res);
						state_stack.pop();
						word.clear();
					}
					else
						continue;
				}
				else if (c == '\'')
					state_stack.push(STRING_SIMPLE);
				else if (c == '"')
					state_stack.push(STRING_DOUBLE);
				else if (c == '\\')
					state_stack.push(ESCAPE);
				else
					word += c;
				break;
			}
			case SERVER_NAME:
			{
				if (std::isblank(c) || c == ';')
				{
					if (c == ';' && word.empty() && this->_server_names.empty())
						throw WebServConfig::SyntaxException(info, "Unexpected '" + std::string(1, c) + "' missing server name");
					else if (!word.empty())
					{
						if (word.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-.") != std::string::npos)
							throw WebServConfig::SyntaxException(info, "Invalid server name: " + word + " (must be a valid hostname)");
						this->_server_names.push_back(word);
						std::cout << "Added server name: " << word << std::endl;
						if (c == ';')
							state_stack.pop();
						word.clear();
					}
					else
						continue;
				}
				else if (iscntrl(c))
					throw WebServConfig::SyntaxException(info, "Unexpected control character");
				else if (c == '\'')
					state_stack.push(STRING_SIMPLE);
				else if (c == '"')
					state_stack.push(STRING_DOUBLE);
				else if (c == '\\')
					state_stack.push(ESCAPE);
				else
					word += c;
				break;
			}
			case ERROR_PAGE:
			{
				break;
			}
			case CLIENT_MAX_BODY_SIZE:
			{
				break;
			}
			case ROOT:
			{
				break;
			}
			case INDEX:
			{
				break;
			}
			case LOCATION:
			{
				if (std::isspace(c) || c == '{')
				{
					if (word.empty())
						throw WebServConfig::SyntaxException(info, "Unexpected '" + std::string(1, c) + "' missing location path");
					else
					{
						_locations.push_back(LocationConfig(info, word));
						state_stack.pop();
						word.clear();
					}
				}
				else if (c == '#')
					state_stack.push(COMMENT);
				else if (c == '\'')
					state_stack.push(STRING_SIMPLE);
				else if (c == '"')
					state_stack.push(STRING_DOUBLE);
				else if (c == '\\')
					state_stack.push(ESCAPE);
				else
					word += c;
				break;
			}
			default:
				break;
		}
	}
}

ServerConfig::ServerConfig(const ServerConfig &copy):
	_port(copy._port),
	_host(copy._host),
	_server_names(copy._server_names),
	_error_pages(copy._error_pages),
	_client_max_body_size(copy._client_max_body_size),
	_root(copy._root),
	_index(copy._index),
	_locations(copy._locations)
{
}

ServerConfig	&ServerConfig::operator=(const ServerConfig &other)
{
	if (this != &other)
	{
		_port = other._port;
		_host = other._host;
		_server_names = other._server_names;
		_error_pages = other._error_pages;
		_client_max_body_size = other._client_max_body_size;
		_root = other._root;
		_index = other._index;
		_locations = other._locations;
	}
	return *this;
}