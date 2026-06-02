#include "Lexer.hpp"

#include <fstream>
#include <sstream>
#include "Token.hpp"
#include "Segment.hpp"

Lexer::~Lexer()
{
}

Lexer::Lexer(const std::string &filename): _tokens()
{
	std::ifstream	stream(filename.c_str());
	if (!stream.is_open())
	{
		throw std::runtime_error("Failed to open file: " + filename);
	}

	size_t		line_number = 1;
	size_t		column_number = 1;

	LexerState	state = DEFAULT;
	Token		token;
	Segment		segment;
	char		c;

	while (stream.get(c))
	{
		switch (state)
		{
			case DEFAULT:
				if (c == '#')
				{
					flush_segment(token, segment);
					flush_token(_tokens, token);
					_tokens.push_back(Token(Token::COMMENT, line_number, column_number));
					state = COMMENT;
				}
				else if (c == '{')
				{
					flush_segment(token, segment);
					flush_token(_tokens, token);
					_tokens.push_back(Token(Token::LBRACE, line_number, column_number));
				}
				else if (c == '}')
				{
					flush_segment(token, segment);
					flush_token(_tokens, token);
					_tokens.push_back(Token(Token::RBRACE, line_number, column_number));
				}
				else if (c == ';')
				{
					flush_segment(token, segment);
					flush_token(_tokens, token);
					_tokens.push_back(Token(Token::SEMICOLON, line_number, column_number));
				}
				else if (c == '\n')
				{
					flush_segment(token, segment);
					flush_token(_tokens, token);
					_tokens.push_back(Token(Token::NEWLINE, line_number, column_number));
				}
				else if (isspace(static_cast<unsigned char>(c)))
				{
					flush_segment(token, segment);
					flush_token(_tokens, token);
				}
				else if (c == '"')
				{
					flush_segment(token, segment);
					segment.setLineNumber(line_number);
					segment.setColumnNumber(column_number);
					segment.setType(Segment::DQUOTE);
					state = DQUOTE;
				}
				else if (c == '\'')
				{
					flush_segment(token, segment);
					segment.setLineNumber(line_number);
					segment.setColumnNumber(column_number);
					segment.setType(Segment::SQUOTE);
					state = SQUOTE;
				}
				else if (c == '\\')
				{
					flush_segment(token, segment);
					segment.setLineNumber(line_number);
					segment.setColumnNumber(column_number);
					segment.setType(Segment::DEFAULT_ESCAPED);
					state = DEFAULT_ESCAPE;
				}
				else if (isgraph(static_cast<unsigned char>(c)) || static_cast<unsigned char>(c) >= 128)
				{
					if (segment.getType() == Segment::NONE)
					{
						segment.setLineNumber(line_number);
						segment.setColumnNumber(column_number);
						segment.setType(Segment::DEFAULT);
					}
					segment.setContent(segment.getContent() + c);
				}
				else
				{
					throw LexerUnexpectedControlCharacter(filename, line_number, column_number);
				}
				break;
			case SQUOTE:
				if (c == '\'')
				{
					flush_segment(token, segment);
					state = DEFAULT;
				}
				else if (c == '\\')
				{
					flush_segment(token, segment);
					segment.setType(Segment::SQUOTE_ESCAPED);
					segment.setLineNumber(line_number);
					segment.setColumnNumber(column_number);
					state = SQUOTE_ESCAPE;
				}
				else
				{
					segment += c;
				}
				break;
			case DQUOTE:
				if (c == '"')
				{
					flush_segment(token, segment);
					state = DEFAULT;
				}
				else if (c == '\\')
				{
					flush_segment(token, segment);
					segment.setType(Segment::DQUOTE_ESCAPED);
					segment.setLineNumber(line_number);
					segment.setColumnNumber(column_number);
					state = DQUOTE_ESCAPE;
				}
				else
				{
					segment += c;
				}
				break;
			case DEFAULT_ESCAPE:
				segment += c;
				flush_segment(token, segment);
				segment.setType(Segment::DEFAULT);
				segment.setLineNumber(line_number);
				segment.setColumnNumber(column_number);
				state = DEFAULT;
				break;
			case SQUOTE_ESCAPE:
				segment += c;
				flush_segment(token, segment);
				segment.setType(Segment::SQUOTE);
				segment.setLineNumber(line_number);
				segment.setColumnNumber(column_number);
				state = SQUOTE;
				break;
			case DQUOTE_ESCAPE:
				segment += c;
				flush_segment(token, segment);
				segment.setType(Segment::DQUOTE);
				segment.setLineNumber(line_number);
				segment.setColumnNumber(column_number);
				state = DQUOTE;
				break;
			case COMMENT:
				if (c == '\n')
				{
					state = DEFAULT;
				}
				break;
			default:
				break;
		}
		if (c == '\n')
		{
			line_number++;
			column_number = 1;
		}
		else if ((static_cast<unsigned char>(c) & 0xC0) != 0x80)
			column_number++;
	}
	if (state == SQUOTE || state == DQUOTE)
	{
		throw LexerUnexpectedEndOfFile("Unclosed quote", filename, line_number, column_number);
	}
	else if (state == DEFAULT_ESCAPE || state == SQUOTE_ESCAPE || state == DQUOTE_ESCAPE)
	{
		throw LexerUnexpectedEndOfFile("Unexpected end of file after escape character", filename, line_number, column_number);
	}
	flush_segment(token, segment);
	flush_token(_tokens, token);
}

void	Lexer::flush_segment(Token &token, Segment &segment)
{
	if (segment.getType() != Segment::NONE)
	{
		if (token.getType() == Token::NONE)
		{
			token.setType(Token::WORD);
			token.setLineNumber(segment.getLineNumber());
			token.setColumnNumber(segment.getColumnNumber());
		}
		token += segment;
		segment.clear();
	}
}

void	Lexer::flush_token(std::vector<Token> &_tokens, Token &token)
{
	if (token.getType() != Token::NONE)
	{
		_tokens.push_back(token);
		token.clear();
	}
}

const std::vector<Token>	&Lexer::getTokens() const
{
	return _tokens;
}

Lexer::LexerUnexpectedControlCharacter::~LexerUnexpectedControlCharacter() throw()
{
}

Lexer::LexerUnexpectedControlCharacter::LexerUnexpectedControlCharacter(std::string filename, size_t line_number, size_t column_number): _message()
{
	std::stringstream ss;
	ss << filename << ":" << line_number << ":" << column_number << " Unexpected control character";
	_message = ss.str();
}

const char	*Lexer::LexerUnexpectedControlCharacter::what() const throw()
{
	return _message.c_str();
}

Lexer::LexerUnexpectedEndOfFile::~LexerUnexpectedEndOfFile() throw()
{
}

Lexer::LexerUnexpectedEndOfFile::LexerUnexpectedEndOfFile(const std::string &message, std::string filename, size_t line_number, size_t column_number): _message()
{
	std::stringstream ss;
	ss << filename << ":" << line_number << ":" << column_number << " " << message;
	_message = ss.str();
}

const char	*Lexer::LexerUnexpectedEndOfFile::what() const throw()
{
	return _message.c_str();
}