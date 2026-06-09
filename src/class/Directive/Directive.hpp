#pragma once

#include "Segment.hpp"
#include <vector>

class	Directive
{
	private:
		std::vector<Segment>				_name;
		std::vector<std::vector<Segment> >	_args;
		std::vector<Directive>				_children;
		std::string							_filename;
		size_t								_line_number;
		size_t								_column_number;
	public:
		~Directive();
		Directive(const std::vector<Segment> &name, const std::string &filename, size_t line_number, size_t column_number);
		Directive(const Directive &copy);
		Directive	&operator=(const Directive &other);
	
		const std::vector<Segment>				&getName() const;
		const std::vector<std::vector<Segment> >	&getArgs() const;
		const std::vector<Directive>			&getChildren() const;

		std::vector<Directive>					&getChildrenRef();

		void									addArg(const std::vector<Segment> &arg);
		void									addChild(const Directive &child);
		const std::string						&getFilename() const;
		size_t									getLineNumber() const;
		size_t									getColumnNumber() const;
};