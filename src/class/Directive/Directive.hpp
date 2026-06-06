#pragma once

#include "Segment.hpp"
#include <vector>

class	Directive
{
	private:
		std::vector<Segment>				_name;
		std::vector<std::vector<Segment>>	_args;
		std::vector<Directive>				_children;
	public:
		~Directive();
		Directive(const std::vector<Segment> &name);
		Directive(const Directive &copy);
		Directive	&operator=(const Directive &other);
	
		const std::vector<Segment>				&getName() const;
		const std::vector<std::vector<Segment>>	&getArgs() const;
		const std::vector<Directive>			&getChildren() const;

		std::vector<Directive>					&getChildrenRef();

		void									addArg(const std::vector<Segment> &arg);
		void									addChild(const Directive &child);
};