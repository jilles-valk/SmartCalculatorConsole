#pragma once

#include <string>
#include <exception>

struct TreeBuildingException : public std::exception
{
	bool hasValue;
	std::string::const_iterator left;
	std::string::const_iterator right;
	
	TreeBuildingException() : hasValue{ false } {};

	TreeBuildingException(std::string::const_iterator left, std::string::const_iterator right) : hasValue{ true }, left { left }, right{ right } {};

	virtual const char* what() const throw()
	{
		return "Exception during building of tree.";
	}
};

struct ParenthesesException : public TreeBuildingException
{
	ParenthesesException(std::string::const_iterator left, std::string::const_iterator right) : TreeBuildingException{ left, right } {};

	virtual const char* what() const throw()
	{
		return "Exception with parentheses.";
	}
};

struct ValueParseException : public TreeBuildingException
{
	ValueParseException(std::string::const_iterator left, std::string::const_iterator right) : TreeBuildingException{ left, right } {};

	virtual const char* what() const throw()
	{
		return "Exception parsing value.";
	}
};

struct NoValidOperatorException : public TreeBuildingException
{
	NoValidOperatorException(std::string::const_iterator left, std::string::const_iterator right) : TreeBuildingException{ left, right } {};

	virtual const char* what() const throw()
	{
		return "Exception parsing operator.";
	}
};

struct DoubleOperatorException : public TreeBuildingException
{
	DoubleOperatorException(std::string::const_iterator left, std::string::const_iterator right) : TreeBuildingException{ left, right } {};

	virtual const char* what() const throw()
	{
		return "Exception parsing operator.";
	}
};

struct NoRightValueForOperator : public TreeBuildingException
{
	NoRightValueForOperator(std::string::const_iterator left, std::string::const_iterator right) : TreeBuildingException{ left, right } {};

	virtual const char* what() const throw()
	{
		return "Exception no value to the right of operator.";
	}
};
