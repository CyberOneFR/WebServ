#include "LocationConfig.hpp"
#include <fstream>
#include <WebServConfig.hpp>
#include <stack>

LocationConfig::~LocationConfig()
{
}

LocationConfig::LocationConfig(ParsingInfo &info)
{
	std::stack<LocationSyntaxState>	state_stack;
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
			case DEFAULT:
				StateMachineDefault(info, state_stack, word, c);
				break;
		}
	}
}

static void	LocationConfig::StateDefault(ParsingInfo &info, std::stack<LocationSyntaxState> &state_stack, std::string &word, char c)
{
	if (c == '#')
		state_stack.push(COMMENT);
	else if (c == '\'')
		state_stack.push(STRING_SIMPLE);
	else if (c == '"')
		state_stack.push(STRING_DOUBLE);
	else if (c == '\\')
		state_stack.push(ESCAPE);
	else if (std::isspace(c))
	{
		if (!word.empty())
		{
			// Process the word
			word.clear();
		}
	}
	else
		word += c;
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