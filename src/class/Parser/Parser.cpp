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
	std::vector<Token>::const_iterator	end = lexer.getTokens().end();

	parseListDirective(it, end, _directives);
	if (it != end)
	{
		//throw unexpected token (expected end of file)
	}
}

bool	expected(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator &end, Token::Type expected_type)
{
	if (it == end)
	{
		//throw unexpected end of file
	}
	else if (it->getType() != expected_type)
		return true;
	return false;
}

Directive	Parser::parseDirective(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator &end)
{
	Directive	directive(it->getSegments());
	it++;

	while (it != end)
	{
		switch (it->getType())
		{
			case Token::WORD:
				directive.addArg(it->getSegments());
				break;
			case Token::SEMICOLON:
				return directive;
			case Token::NEWLINE:
			case Token::LBRACE:
				parseBlock(it, end, directive.getChildrenRef());
				return directive;
			default:
				//throw unexpected token (expected directive argument, block start or directive end)
				break;
		}
		it++;
	}
}

void	Parser::parseBlock(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator &end, std::vector<Directive> &directives)
{
	while (it != end)
	{
		if (it->getType() != Token::NEWLINE)
			break;
		it++;
	}
	if (expected(it, end, Token::LBRACE))
	{
		//throw unexpected token (expected block start)
	}
	it++;
	parseListDirective(it, end, directives);
	if (expected(it, end, Token::RBRACE))
	{
		//throw unexpected token (expected block end)
	}
	it++;
}

void	Parser::parseListDirective(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator &end, std::vector<Directive> &directives)
{
	while (it != end)
	{
		switch (it->getType())
		{
			case Token::WORD:
				directives.push_back(parseDirective(it, end));
				break;
			case Token::NEWLINE:
				break;
			default:
				return;
		}
		it++;
	}
}
