#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <cctype>
#include <cstring>
#include <variant>

#include "operator.hpp"

class Token;

class IncludeClass
{
public:
	static bool Match(std::vector<Token>* tokens);

	//friend std::ostream& operator<<(std::ostream& o, IncludeClass& ic);
	void output(std::ostream& o, int rule);

	enum
	{
		Module,
		Include
	};

private:
	std::string path;
};

class Expression
{
public:
	static bool MatchOperator(std::vector<Token>* tokens);

	friend std::ostream& operator<<(std::ostream& o, Expression& e);

	enum
	{
		NumberLiteral,
		StringLiteral,
		Identifier,
		Operation
	};

	int rule = -1;

	std::variant<
		std::string,
		std::vector<Expression>> value;
};

typedef std::variant<
	IncludeClass,
	Expression> ValueType;

class Token
{
public:
	Token(int loc, int type, int rule, ValueType v)
	: loc(loc), type(type), rule(rule), value(v) {} 

	Token(int loc, int type, int rule)
	: loc(loc), type(type), rule(rule) {}

	friend std::ostream& operator<<(std::ostream& o, Token& t);

	enum
	{
		Atom,
		Include,
		Expression
	};

	int loc = 0;
	int type = -1;
	int rule = -1;

	ValueType value;
};

#endif