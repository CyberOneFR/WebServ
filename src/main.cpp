#include <iostream>
#include "Lexer.hpp"

int	main(int argc, char **argv) {
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return (1);
	}
	try {
		// Lexer
	}
	catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}