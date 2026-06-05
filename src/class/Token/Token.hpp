#pragma once

#include "Segment.hpp"
#include <string>
#include <vector>

class	Token
{
	public:
		enum Type
		{
			NONE,
			WORD,
			LBRACE,
			RBRACE,
			SEMICOLON,
			NEWLINE,
		};
	private:
		Type					_type;
		std::vector<Segment>	_segments;
		size_t					_line_number;
		size_t					_column_number;
	public:
		~Token();
		Token();
		Token(Type type, size_t line_number, size_t column_number);
		Token(const Token &copy);
		Token	&operator=(const Token &other);

		Token	&operator+=(const Segment &segment);
		void	clear();

		const std::string			getTypeString() const;

		Type						getType() const;
		const std::vector<Segment>	&getSegments() const;
		size_t						getLineNumber() const;
		size_t						getColumnNumber() const;

		void	setType(Type type);
		void	setLineNumber(size_t line_number);
		void	setColumnNumber(size_t column_number);
};