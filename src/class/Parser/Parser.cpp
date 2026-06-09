#include "Directive.hpp"
#include "Segment.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include <iterator>
#include <assert.h>
#include <sstream>
#include <iostream>

Parser::~Parser()
{
}

/*
parser := list_directive EOF
*/
Parser::Parser(const Lexer &lexer): _directives()
{
	std::vector<Token>::const_iterator	it = lexer.getTokens().begin();
	std::vector<Token>::const_iterator	end = lexer.getTokens().end();

	parseListDirective(it, end, _directives);
	if (it->getType() != Token::_EOF)
		throw ParserUnexpectedToken("end of file", it->getFilename(), it->getLineNumber(), it->getColumnNumber());
}

/*
directive := WORD WORD* (SEMICOLON | block)
*/
Directive	Parser::parseDirective(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator &end)
{
	Directive	directive(it->getSegments(), it->getFilename(), it->getLineNumber(), it->getColumnNumber());
	it++;

	while (it->getType() != Token::_EOF)
	{
		switch (it->getType())
		{
			case Token::WORD:
				directive.addArg(it->getSegments());
				it++;
				break;
			case Token::SEMICOLON:
				it++;
				return directive;
			case Token::NEWLINE:
			case Token::LBRACE:
				parseBlock(it, end, directive.getChildrenRef());
				return directive;
			default:
				throw ParserUnexpectedToken("directive argument, block start or directive end", it->getFilename(), it->getLineNumber(), it->getColumnNumber());
		}
	}
	throw ParserUnexpectedEndOfFile("directive argument, block start or directive end", it->getFilename(), it->getLineNumber(), it->getColumnNumber());
}

/*
block := NEWLINE* LBRACE list_directive RBRACE
*/
void	Parser::parseBlock(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator &end, std::vector<Directive> &directives)
{
	while (it->getType() != Token::_EOF)
	{
		if (it->getType() == Token::NEWLINE)
			it++;
		else
			break;
	}
	if (it->getType() == Token::LBRACE)
		it++;
	else if (it->getType() == Token::_EOF)
		throw ParserUnexpectedEndOfFile("block start", it->getFilename(), it->getLineNumber(), it->getColumnNumber());
	else
		throw ParserUnexpectedToken("block start", it->getFilename(), it->getLineNumber(), it->getColumnNumber());
	parseListDirective(it, end, directives);
	if (it->getType() == Token::RBRACE)
		it++;
	else if (it->getType() == Token::_EOF)
		throw ParserUnexpectedEndOfFile("}", it->getFilename(), it->getLineNumber(), it->getColumnNumber());
	else
		throw ParserUnexpectedToken("}", it->getFilename(), it->getLineNumber(), it->getColumnNumber());
}

/*
list_directive := directive*
*/
void	Parser::parseListDirective(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator &end, std::vector<Directive> &directives)
{
	while (it->getType() != Token::_EOF)
	{
		switch (it->getType())
		{
			case Token::WORD:
				directives.push_back(parseDirective(it, end));
				break;
			case Token::NEWLINE:
				it++;
				break;
			default:
				return;
		}
	}
}

const std::vector<Directive>	&Parser::getDirectives() const
{
	return _directives;
}

Parser::ParserUnexpectedToken::~ParserUnexpectedToken() throw()
{
}

Parser::ParserUnexpectedToken::ParserUnexpectedToken(const std::string &expected, const std::string &filename, size_t line_number, size_t column_number): _message()
{
	std::stringstream	ss;
	ss << filename << ":" << line_number << ":" << column_number << " Unexpected token (expected: " << expected << ")";
	_message = ss.str();
}

Parser::ParserUnexpectedEndOfFile::~ParserUnexpectedEndOfFile() throw()
{
}

Parser::ParserUnexpectedEndOfFile::ParserUnexpectedEndOfFile(const std::string &expected, const std::string &filename, size_t line_number, size_t column_number): _message()
{
	std::stringstream	ss;
	ss << filename << ":" << line_number << ":" << column_number << " Unexpected end of file (expected: " << expected << ")";
	_message = ss.str();
}

const char	*Parser::ParserUnexpectedToken::what() const throw()
{
	return _message.c_str();
}

const char	*Parser::ParserUnexpectedEndOfFile::what() const throw()
{
	return _message.c_str();
}