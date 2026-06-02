#include "Segment.hpp"

Segment::~Segment()
{
}

Segment::Segment(): _type(NONE), _content(), _line_number(0), _column_number(0)
{
}

Segment::Segment(const Segment &copy): _type(copy._type), _content(copy._content), _line_number(copy._line_number), _column_number(copy._column_number)
{
}

Segment	&Segment::operator=(const Segment &other)
{
	if (this != &other)
	{
		_content = other._content;
		_type = other._type;
		_line_number = other._line_number;
		_column_number = other._column_number;
	}
	return *this;
}

Segment	&Segment::operator+=(const char c)
{
	_content += c;
	return *this;
}

void	Segment::clear()
{
	_content.clear();
	_type = NONE;
	_line_number = 0;
	_column_number = 0;
}

Segment::Type	Segment::getType() const
{
	return _type;
}

const std::string	&Segment::getContent() const
{
	return _content;
}

size_t	Segment::getLineNumber() const
{
	return _line_number;
}

size_t	Segment::getColumnNumber() const
{
	return _column_number;
}

void	Segment::setType(Type type)
{
	_type = type;
}

void	Segment::setContent(const std::string &content)
{
	_content = content;
}

void	Segment::setLineNumber(size_t line_number)
{
	_line_number = line_number;
}

void	Segment::setColumnNumber(size_t column_number)
{
	_column_number = column_number;
}