#pragma once

#include "Segment.hpp"
#include <vector>

class	Directive
{
	private:
		std::vector<Segment>				_segment;
		std::vector<std::vector<Segment>>	_args;
		std::vector<Directive>				_children;
	public:
		~Directive();
		Directive(const std::vector<Segment> &segment);
		Directive(const Directive &copy);
		Directive	&operator=(const Directive &other);
	
		const std::vector<Segment>				&getSegment() const;
		const std::vector<std::vector<Segment>>	&getArgs() const;

		std::vector<Directive>			&getChildren() const;

		void	addArg(const std::vector<Segment> &arg);
		void	addChild(const Directive &child);
};