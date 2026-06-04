#pragma once

#include "Token.hpp"
#include "Directive.hpp"
#include "WebServConfig.hpp"

class	Parser
{
	private:
		enum	ParserState
		{
			DEFAULT,
			ARGUMENTS,
		};
		std::vector<Directive>	_directives;
	public:
		~Parser();
		Parser(const std::string &filename);

		void	parseDirective(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator end, std::vector<Directive> &directives);
};