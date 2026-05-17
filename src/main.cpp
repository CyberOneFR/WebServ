#include <iostream>
#include "WebServ.hpp"

int	main(int argc, char **argv) {
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return (1);
	}
	try {
		WebServ	server(argv[1]);
		std::cout << "WebServ started successfully." << std::endl;
	}
	catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}