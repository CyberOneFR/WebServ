#include "Parser.hpp"
#include "Lexer.hpp"
#include <iterator>
#include "Segment.hpp"

Parser::~Parser()
{
}

Parser::Parser(const std::string &filename)
{
	Lexer								lexer(filename);
	std::vector<Token>::const_iterator	it = lexer.getTokens().begin();

	parseDirective(it, lexer.getTokens().end(), _directives);
}

void	Parser::parseDirective(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator end, std::vector<Directive> &directives)
{
	ParserState	state = DEFAULT;

	while (it != end)
	{
		Token	token = *it;
		switch(state)
		{
			case DEFAULT:
				if (token.getType() == Token::WORD)
				{
					_directives.push_back(Directive(token.getSegments()));
					state = ARGUMENTS;
				}
				else if (token.getType() == Token::LBRACE)
				{
					// Handle LBRACE token
				}
				else if (token.getType() == Token::RBRACE)
				{
					// Handle RBRACE token
				}
				else if (token.getType() == Token::SEMICOLON)
				{
					// Handle SEMICOLON token
				}
				else if (token.getType() == Token::NEWLINE)
				{
					// Handle NEWLINE token
				}
				else if (token.getType() == Token::WHITESPACE)
				{
					// Handle WHITESPACE token
				}
				else
				{
					// Handle unexpected token type
				}
				break;
			case ARGUMENTS:
				if (token.getType() == Token::WORD)
				{
					_directives.back().addArg(token.getSegments());
				}
				else if (token.getType() == Token::LBRACE)
				{
					parseDirective(++it, end, _directives.back().getChildren());
				}
				else if (token.getType() == Token::RBRACE)
				{
					// Handle RBRACE token in ARGUMENTS state
				}
				else if (token.getType() == Token::SEMICOLON)
				{
					// Handle SEMICOLON token in ARGUMENTS state
				}
				else if (token.getType() == Token::NEWLINE)
				{
					// Handle NEWLINE token in ARGUMENTS state
				}
				else if (token.getType() == Token::WHITESPACE)
				{
					// Handle WHITESPACE token in ARGUMENTS state
				}
				else
				{
					// Handle unexpected token type in ARGUMENTS state
				}
		}
		it++;
	}
}