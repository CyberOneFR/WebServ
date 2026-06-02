#pragma once

#include <fstream>
#include <vector>
#include "Token.hpp"

class	Lexer
{
	private:
		enum	LexerState
		{
			DEFAULT,
			DEFAULT_ESCAPE,
			SQUOTE,
			SQUOTE_ESCAPE,
			DQUOTE,
			DQUOTE_ESCAPE,
			COMMENT,
		};
		std::vector<Token>	_tokens;

		void	flush_segment(Token &token, Segment &segment);
		void	flush_token(std::vector<Token> &_tokens, Token &token);
	public:
		~Lexer();
		Lexer(const std::string &filename);

		const std::vector<Token>	&getTokens() const;

		class	LexerUnexpectedControlCharacter: public std::exception
		{
			private:
				std::string		_message;
			public:
				virtual ~LexerUnexpectedControlCharacter() throw();
				LexerUnexpectedControlCharacter(std::string filename, size_t line_number, size_t column_number);
				virtual const char	*what() const throw();
		};
		class	LexerUnexpectedEndOfFile: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~LexerUnexpectedEndOfFile() throw();
				LexerUnexpectedEndOfFile(const std::string &message, std::string filename, size_t line_number, size_t column_number);
				virtual const char	*what() const throw();
		};
};