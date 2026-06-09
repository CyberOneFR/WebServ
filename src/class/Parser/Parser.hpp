#pragma once

#include "Directive.hpp"
#include "Token.hpp"
#include "Lexer.hpp"
#include <vector>
#include <exception>
#include <string>

class	Parser
{
	private:
		std::vector<Directive>	_directives;

		Directive	parseDirective(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator &end);
		void		parseBlock(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator &end, std::vector<Directive> &directives);
		void		parseListDirective(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator &end, std::vector<Directive> &directives);

	public:
		~Parser();
		Parser(const Lexer &lexer);

		const std::vector<Directive>	&getDirectives() const;

		class	ParserUnexpectedToken: public std::exception
		{
			private:
				std::string		_message;
			public:
				virtual ~ParserUnexpectedToken() throw();
				ParserUnexpectedToken(const std::string &expected, const std::string &filename, size_t line_number, size_t column_number);
				virtual const char	*what() const throw();
		};
		class	ParserUnexpectedEndOfFile: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~ParserUnexpectedEndOfFile() throw();
				ParserUnexpectedEndOfFile(const std::string &expected, const std::string &filename, size_t line_number, size_t column_number);
				virtual const char	*what() const throw();
		};
};