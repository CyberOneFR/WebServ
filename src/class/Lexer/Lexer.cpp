#include "Lexer.hpp"

#include <fstream>

Lexer::~Lexer()
{
}

Lexer::Lexer(std::ifstream &stream)
{
	size_t		line_number = 1;
	size_t		column_number = 0;

	LexerState	state = DEFAULT;
	LexerState	after_escape = DEFAULT;
	std::string	word;
	char		c;

	while (stream.get(c))
	{
		switch(state)
		{
			case DEFAULT:
			{
				if (c == '#')
				{
					state = COMMENT;
				}
				else if (c == '\'')
				{
					state = SQUOTE_STRING;
				}
				else if (c == '\"')
				{
					state = DQUOTE_STRING;
				}
				else if (c == '\\')
				{
					after_escape = state;
					state = ESCAPE;
				}
				else if (c == '{')
				{
					_tokens.push_back({Token::LBRACE, "{", line_number, column_number});
				}
				else if (c == '}')
				{
					_tokens.push_back({Token::RBRACE, "}", line_number, column_number});
				}
				else if (c == ';')
				{
					_tokens.push_back({Token::SEMICOLON, ";", line_number, column_number});
				}
				else if (c == '\n')
				{
					_tokens.push_back({Token::NEWLINE, "\n", line_number, column_number});
					line_number++;
					column_number = 0;
				}
				else if (isblank(c))
				{
					_tokens.push_back({Token::WHITESPACE, " ", line_number, column_number});
				}
				else
				{
					word += c;
					state = WORD;
				}
				break;
			}
			case COMMENT:
			{
				if (c == '\n')
				{
					_tokens.push_back({Token::COMMENT, word, line_number, column_number});
					_tokens.push_back({Token::NEWLINE, "\n", line_number, column_number});
					state = DEFAULT;
					line_number++;
					column_number = 0;
					word.clear();
				}
				else
					word += c;
				break;
			}
			case SQUOTE_STRING:
			{
				if (c == '\'')
				{
					_tokens.push_back({Token::SQUOTE_STRING, word, line_number, column_number});
					state = DEFAULT;
					word.clear();
				}
				else if (c == '\\')
				{
					after_escape = state;
					state = ESCAPE;
				}
				else
					word += c;
				break;
			}
			case DQUOTE_STRING:
			{
				if (c == '\"')
				{
					_tokens.push_back({Token::DQUOTE_STRING, word, line_number, column_number});
					state = DEFAULT;
					word.clear();
				}
				else if (c == '\\')
				{
					after_escape = state;
					state = ESCAPE;
				}
				else
					word += c;
				break;
			}
			case ESCAPE:
			{
				word += c;
				state = after_escape;
				break;
			}
			case WORD:
			{
				if (c == '#')
				{
					_tokens.push_back({Token::WORD, word, line_number, column_number});
					word.clear();
					state = COMMENT;
				}
				else if (c == '\'')
				{
					state = SQUOTE_STRING;
				}
				else if (c == '\"')
				{
					state = DQUOTE_STRING;
				}
				else if (c == '\\')
				{
					after_escape = state;
					state = ESCAPE;
				}
				if (isblank(c))
				{
					_tokens.push_back({Token::WORD, word, line_number, column_number});
					word.clear();
					state = DEFAULT;
					if (c == '{')
					{
						_tokens.push_back({Token::LBRACE, "{", line_number, column_number});
					}
					else if (c == '}')
					{
						_tokens.push_back({Token::RBRACE, "}", line_number, column_number});
					}
					else if (c == ';')
					{
						_tokens.push_back({Token::SEMICOLON, ";", line_number, column_number});
					}
					else if (c == '\n')
					{
						_tokens.push_back({Token::NEWLINE, "\n", line_number, column_number});
						line_number++;
						column_number = 0;
					}
				}
				
				else
					word += c;
				break;
			}
			default:
				break;
		}
	}
}
