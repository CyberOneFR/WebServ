#include "WebServConfig.hpp"
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <stack>

enum WebServSyntaxState
{
	DEFAULT,
	WORD,
	COMMENT,
	STRING_SIMPLE,
	STRING_DOUBLE,
	ESCAPE,
};

WebServConfig::~WebServConfig()
{
}

void	WebServConfig::handle_word(ParsingInfo &info, std::string &word)
{
	std::cout << word << std::endl;
	if (word.compare("server") == 0)
		_servers.push_back(ServerConfig(info.stream));
	else if (word.empty())
	{
		throw WebServSyntaxException(info, "Unexpected '{'");
	}
	else
		throw WebServSyntaxException(info, "Unknown directive: " + word);
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
	std::string line;
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
				else if (c == '{')
					handle_word(info, word);
				else if (c == '}')
					throw WebServSyntaxException(info, "Unexpected '}'");
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
					handle_word(info, word);
				}
				else if (c == '#')
				{
					state_stack.pop();
					state_stack.push(COMMENT);
				}
				else if (c == '{')
					handle_word(info, word);
				else if (c == '}')
					throw WebServSyntaxException(info, "Unexpected '}'");
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

WebServConfig::WebServSyntaxException::~WebServSyntaxException() throw()
{
}

WebServConfig::WebServSyntaxException::WebServSyntaxException(ParsingInfo &info, const std::string &message)
{
	std::stringstream ss;
	ss << info.path << ":" << info.line_number << ":" << info.column_number << " " << message;
	_message = ss.str();
}

WebServConfig::WebServSyntaxException::WebServSyntaxException(const WebServSyntaxException &copy): _message(copy._message)
{
}

WebServConfig::WebServSyntaxException	&WebServConfig::WebServSyntaxException::operator=(const WebServSyntaxException &other)
{
	if (this != &other)
	{
		_message = other._message;
	}
	return *this;
}

const char	*WebServConfig::WebServSyntaxException::what(void) const throw()
{
	return _message.c_str();
}
