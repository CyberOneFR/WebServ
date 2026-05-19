#include "WebServConfig.hpp"
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <stack>

WebServConfig::~WebServConfig()
{
}

void	WebServConfig::handle_directive(ParsingInfo &info, std::stack<WebServSyntaxState> &state_stack, std::string &word, char c)
{
	if (c == '{')
	{
		if (word.compare("server") == 0)
			_servers.push_back(ServerConfig(info));
		else if (word.empty())
			throw SyntaxException(info, "Unexpected '" + std::string(1, c) + "'");
		else
			throw SyntaxException(info, "Unknown directive: " + word);
	}
	else if (isspace(c))
	{
		if (word.compare("server") == 0)
			state_stack.push(SERVER);
		else if (word.empty())
			throw SyntaxException(info, "Unexpected '" + std::string(1, c) + "'");
		else
			throw SyntaxException(info, "Unknown directive: " + word);
	}
	else
		throw SyntaxException(info, "Unexpected '" + std::string(1, c) + "'");
	word.clear();
}

WebServConfig::WebServConfig(const std::string &path)
{
	ParsingInfo						info;
	std::stack<WebServSyntaxState>	state_stack;
	std::string						word;
	char							c;

	info.stream.open(path.c_str());
	if (!info.stream)
		throw std::runtime_error("Can't open " + path + ": " + std::strerror(errno));
	info.path = path;
	info.line_number = 1;
	info.column_number = 0;
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
			case DEFAULT:
			{
				if (std::isspace(c))
					continue;
				else if (c == '#')
					state_stack.push(COMMENT);
				else if (c == '{' || c == ';' || c == '}')
					handle_directive(info, state_stack, word, c);
				else if (c == '\'')
					state_stack.push(STRING_SIMPLE);
				else if (c == '"')
					state_stack.push(STRING_DOUBLE);
				else if (c == '\\')
					state_stack.push(ESCAPE);
				else
				{
					word += c;
					state_stack.push(WORD);
				}
				break;
			}
			case WORD:
			{
				if (std::isspace(c))
				{
					state_stack.pop();
					handle_directive(info, state_stack, word, c);
				}
				else if (c == '#')
				{
					state_stack.pop();
					state_stack.push(COMMENT);
				}
				else if (c == '{' || c == ';' || c == '}')
					handle_directive(info, state_stack, word, c);
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
			case COMMENT:
			{
				if (c == '\n')
					state_stack.pop();
				break;
			}
			case STRING_SIMPLE:
			{
				if (c == '\'')
					state_stack.pop();
				else if (c == '\\')
					state_stack.push(ESCAPE);
				else
					word += c;
				break;
			}
			case STRING_DOUBLE:
			{
				if (c == '"')
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
			case SERVER:
			{
				if (isspace(c))
					continue;
				else if (c == '{')
				{
					_servers.push_back(ServerConfig(info));
					state_stack.pop();
				}
				else if (c == '#')
					state_stack.push(COMMENT);
				else
					throw SyntaxException(info, "Unexpected '" + std::string(1, c) + "' in server block");
			}
			default:
				break;
		}
	}
}

WebServConfig::WebServConfig(const WebServConfig &copy): _servers(copy._servers)
{
}

WebServConfig	&WebServConfig::operator=(const WebServConfig &other)
{
	if (this != &other)
	{
		_servers = other._servers;
	}
	return *this;
}

WebServConfig::SyntaxException::~SyntaxException() throw()
{
}

WebServConfig::SyntaxException::SyntaxException(ParsingInfo &info, const std::string &message)
{
	std::stringstream ss;
	ss << info.path << ":" << info.line_number << ":" << info.column_number << " " << message;
	_message = ss.str();
}

WebServConfig::SyntaxException::SyntaxException(const SyntaxException &copy): _message(copy._message)
{
}

WebServConfig::SyntaxException	&WebServConfig::SyntaxException::operator=(const SyntaxException &other)
{
	if (this != &other)
	{
		_message = other._message;
	}
	return *this;
}

const char	*WebServConfig::SyntaxException::what(void) const throw()
{
	return _message.c_str();
}
