#include "Directive.hpp"
#include "Segment.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include <iterator>

Parser::~Parser()
{
}

Parser::Parser(const std::string &filename)
{
	Lexer								lexer(filename);
	std::vector<Token>::const_iterator	it = lexer.getTokens().begin();

	parseListDirective(it, lexer.getTokens().end(), _directives);
}



void	Parser::parseDirective(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator end)
{
	Directive	directive(it->getSegments());

	while (it != end)
	{
		switch (it->getType())
		{
			case Token::WORD:
				break;
			case Token::SEMICOLON:
				return;
				break;
			case Token::LBRACE:
				parseListDirective(it, end, directive.getChildrenRef());
				break;
			case Token::RBRACE:
				return;
			default:
				return;
		}
	}
}

void	Parser::parseListDirective(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator end, std::vector<Directive> &directives)
{
	while (it != end)
	{
		switch (it->getType())
		{
			case Token::WORD:
				parseDirective(it, end, directives.emplace_back());
				break;
			default:
				return;
				break;
		}
	}
}
