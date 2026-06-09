#include <iostream>
#include <sstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Debug.hpp"

void	printListDirective(const std::vector<Directive> &directives)
{
	std::vector<Directive>::const_iterator	it = directives.begin();
	std::vector<Directive>::const_iterator	end = directives.end();
	while (it != end)
	{
		const Directive &directive = *it;
		std::cout << "DIRECTIVE [" << directive.getName()[0].getContent() << "]" << std::endl;
		for (size_t i = 0; i < directive.getArgs().size(); ++i)
		{
			std::cout << "  Arg: ";
			for (size_t j = 0; j < directive.getArgs()[i].size(); ++j)
			{
				std::cout << directive.getArgs()[i][j].getContent() << " ";
			}
			std::cout << std::endl;
		}
		printListDirective(directive.getChildren());
		it++;
	}
}

int	main(int argc, char **argv) {
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return (1);
	}
	try {
		Lexer	lexer(argv[1]);
		Debug::printLexer(std::cout, lexer);
		Parser	parser(lexer);
		Debug::printParser(std::cout, parser);
	}
	catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}