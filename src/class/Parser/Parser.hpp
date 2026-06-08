#pragma once

#include "Token.hpp"
#include "Directive.hpp"
#include "WebServConfig.hpp"

class	Parser
{
	private:
		std::vector<Directive>	_directives;

		Directive	parseDirective(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator &end);
		void		parseBlock(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator &end, std::vector<Directive> &directives);
		void		parseListDirective(std::vector<Token>::const_iterator &it, std::vector<Token>::const_iterator &end, std::vector<Directive> &directives);

	public:
		~Parser();
		Parser(const std::string &filename);

};