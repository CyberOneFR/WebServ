#include "Token.hpp"

Token::~Token()
{
}

Token::Token(): _type(NONE), _segments(), _line_number(0), _column_number(0)
{
}

Token::Token(Type type, size_t line_number, size_t column_number): _type(type), _segments(), _line_number(line_number), _column_number(column_number)
{
}

Token::Token(const Token &copy): _type(copy._type), _segments(copy._segments), _line_number(copy._line_number), _column_number(copy._column_number)
{
}

Token	&Token::operator=(const Token &other)
{
	if (this != &other)
	{
		_type = other._type;
		_segments = other._segments;
		_line_number = other._line_number;
		_column_number = other._column_number;
	}
	return *this;
}

Token	&Token::operator+=(const Segment &segment)
{
	_segments.push_back(segment);
	return *this;
}

void	Token::clear()
{
	_type = NONE;
	_segments.clear();
	_line_number = 0;
	_column_number = 0;
}

const std::string	Token::getTypeString() const
{
	switch (_type)
	{
		case NONE:
			return "NONE";
		case WORD:
			return "WORD";
		case COMMENT:
			return "COMMENT";
		case LBRACE:
			return "LBRACE";
		case RBRACE:
			return "RBRACE";
		case SEMICOLON:
			return "SEMICOLON";
		case NEWLINE:
			return "NEWLINE";
		case WHITESPACE:
			return "WHITESPACE";
		default:
			return "UNKNOWN";
	}
}

Token::Type	Token::getType() const
{
	return _type;
}

const std::vector<Segment>	&Token::getSegments() const
{
	return _segments;
}

size_t	Token::getLineNumber() const
{
	return _line_number;
}

size_t	Token::getColumnNumber() const
{
	return _column_number;
}

void	Token::setType(Type type)
{
	_type = type;
}

void	Token::setLineNumber(size_t line_number)
{
	_line_number = line_number;
}

void	Token::setColumnNumber(size_t column_number)
{
	_column_number = column_number;
}