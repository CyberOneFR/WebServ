#include "Segment.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include <iterator>

Parser::~Parser()
{
}

/*
config := list_directive EOF
list_directive := directive*
directive := WORD WORD* (SEMICOLON | LBRACE list_directive RBRACE)
*/
Parser::Parser(const std::string &filename)
{
	Lexer								lexer(filename);
	const std::vector<Token>			&tokens = lexer.getTokens();
	std::vector<Token>::const_iterator	it = tokens.begin();
}

void	Parser::parseDirective(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator end, Directive &directive)
{
	
}
