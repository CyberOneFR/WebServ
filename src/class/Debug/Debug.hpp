#pragma once

#include "Segment.hpp"
#include "Token.hpp"
#include "Lexer.hpp"
#include "Directive.hpp"
#include "Parser.hpp"
#include <ostream>

class	Debug
{
	public:
		static void	printSegment(std::ostream &os, const Segment &segment);
		static void	printToken(std::ostream &os, const Token &token);
		static void	printLexer(std::ostream &os, const Lexer &lexer);
		static void	printDirective(std::ostream &os, const Directive &directive, size_t indent);
		static void	printParser(std::ostream &os, const Parser &parser);
};