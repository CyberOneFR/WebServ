#pragma once

#include <fstream>
#include <vector>

class	Lexer
{
	enum	LexerState
	{
		DEFAULT,
		COMMENT,
		ESCAPE,
		WORD,
		DQUOTE_STRING,
		SQUOTE_STRING
	};
	struct Token
	{
		enum Type
		{
			WORD,
			DQUOTE_STRING,
			SQUOTE_STRING,
			COMMENT,
			WHITESPACE,
			LBRACE,
			RBRACE,
			SEMICOLON,
			NEWLINE
		}			type;
		std::string	content;
		size_t		line_number;
		size_t		column_number;
	};
	private:
		std::vector<Token>	_tokens;
	public:
		~Lexer();
		Lexer(std::ifstream &stream);

		std::vector<Token>	getTokens() const;
};