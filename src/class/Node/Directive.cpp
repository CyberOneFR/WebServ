#include "Directive.hpp"

Directive::~Directive()
{
}

Directive::Directive(const std::vector<Segment> &segment): _segment(segment), _args(), _children()
{
}

Directive::Directive(const Directive &copy): _segment(copy._segment), _args(copy._args), _children(copy._children)
{
}

Directive	&Directive::operator=(const Directive &other)
{
	if (this != &other)
	{
		_segment = other._segment;
		_args = other._args;
		_children = other._children;
	}
	return (*this);
}

const std::vector<Segment>				&Directive::getSegment() const
{
	return (_segment);
}

const std::vector<std::vector<Segment>>	&Directive::getArgs() const
{
	return (_args);
}

const std::vector<Directive>			&Directive::getChildren() const
{
	return (_children);
}

void	Directive::addArg(const std::vector<Segment> &arg)
{
	_args.push_back(arg);
}

void	Directive::addChild(const Directive &child)
{
	_children.push_back(child);
}