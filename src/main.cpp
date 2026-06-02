#include <iostream>
#include <sstream>
#include "Lexer.hpp"

int	main(int argc, char **argv) {
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return (1);
	}
	try {
		Lexer	lexer(argv[1]);
		const std::vector<Token>	&tokens = lexer.getTokens();
		for (size_t i = 0; i < tokens.size(); ++i)
		{
			const Token &token = tokens[i];
			std::stringstream	ss;
			ss << argv[1] << ":" << token.getLineNumber() << ":" << token.getColumnNumber() << "	"
			<< token.getTypeString() << std::endl;
			for (size_t j = 0; j < token.getSegments().size(); ++j)
			{
				const Segment &segment = token.getSegments()[j];
				ss << "	[" << segment.getTypeString() << " \"" << segment.getContent() << "\"]" << std::endl;
			}
			std::cout << ss.str();
		}
	}
	catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}