#include "Debug.hpp"
#include "Segment.hpp"
#include "Token.hpp"
#include "Lexer.hpp"
#include "Directive.hpp"
#include "Parser.hpp"

void	Debug::printSegment(std::ostream &os, const Segment &segment)
{
	os << segment.getFilename() << ":" << segment.getLineNumber() << ":" << segment.getColumnNumber() << "	[" << segment.getTypeString() << " \"" << segment.getContent() << "\"]" << std::endl;
}

void	Debug::printToken(std::ostream &os, const Token &token)
{
	os << token.getFilename() << ":" << token.getLineNumber() << ":" << token.getColumnNumber() << "	" << token.getTypeString() << std::endl;
	for (size_t j = 0; j < token.getSegments().size(); ++j)
	{
		const Segment &segment = token.getSegments()[j];
		printSegment(os, segment);
	}
}

void	Debug::printLexer(std::ostream &os, const Lexer &lexer)
{
	const std::vector<Token>	&tokens = lexer.getTokens();

	os << "Lexer:" << std::endl;
	for (size_t i = 0; i < tokens.size(); ++i)
	{
		const Token &token = tokens[i];
		printToken(os, token);
	}
	os << std::endl;
}

void	Debug::printDirective(std::ostream &os, const Directive &directive, size_t indent)
{
	os << directive.getFilename() << ":" << directive.getLineNumber() << ":" << directive.getColumnNumber() << "	";
	for (size_t i = 0; i < indent; ++i)
		os << "	";
	os << "DIRECTIVE [" << directive.getName()[0].getContent() << "]" << std::endl;
	for (size_t i = 0; i < directive.getArgs().size(); ++i)
	{
		for (size_t j = 0; j < indent + 3; ++j)
			os << "	";
		os << "Arg: ";
		for (size_t j = 0; j < directive.getArgs()[i].size(); ++j)
		{
			os << directive.getArgs()[i][j].getContent() << "";
		}
		os << std::endl;
	}
	if (directive.getChildren().size() > 0)
	{
		for (size_t i = 0; i < indent + 3; ++i)
			os << "	";
		os << "Children:" << std::endl;
	}
	for (size_t i = 0; i < directive.getChildren().size(); ++i)
	{
		printDirective(os, directive.getChildren()[i], indent + 1);
	}
}

void	Debug::printParser(std::ostream &os, const Parser &parser)
{
	const std::vector<Directive>	&directives = parser.getDirectives();

	os << "Parser:" << std::endl;
	for (size_t i = 0; i < directives.size(); ++i)
	{
		printDirective(os, directives[i], 0);
	}
	os << std::endl;
}