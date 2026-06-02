#pragma once

#include <string>

class	Segment
{
	public:
		enum Type
		{
			NONE,
			DEFAULT,
			DEFAULT_ESCAPED,
			SQUOTE,
			SQUOTE_ESCAPED,
			DQUOTE,
			DQUOTE_ESCAPED
		};
	private:
		Type		_type;
		std::string	_content;
		size_t		_line_number;
		size_t		_column_number;
	public:
		~Segment();
		Segment();
		Segment(const Segment &copy);
		Segment	&operator=(const Segment &other);

		Segment	&operator+=(const char c);
		void	clear();
	
		Type				getType() const;
		const std::string	&getContent() const;
		size_t				getLineNumber() const;
		size_t				getColumnNumber() const;
	
		void	setType(Type type);
		void	setContent(const std::string &content);
		void	setLineNumber(size_t line_number);
		void	setColumnNumber(size_t column_number);
};