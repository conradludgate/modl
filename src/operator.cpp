#include "operator.hpp"
#include "atom.hpp"

#include <map>

#define EXPR (Token::Expression)
#define OP (Expression::Operation)

bool FirstPass(std::vector<Token>* tokens);
bool SecondPass(std::vector<Token>* tokens);
bool ThirdPass(std::vector<Token>* tokens);
bool FourthPass(std::vector<Token>* tokens);

bool Expression::MatchOperator(std::vector<Token>* tokens)
{
	if (tokens->size() < 2) {
		return false;
	}

	// Op::InNamespace - Op::InDot Left to Right
	if (FirstPass(tokens)) 
	{
		return true;
	}
	// Op::PreIncrement - Op::Prereference Right to Left
	else if (SecondPass(tokens)) 
	{
		return true;
	} 
	// Op::InMultiply - Op::InOr Left to Right
	else if (ThirdPass(tokens)) 
	{
		return true;
	} 
	// Op::InAssign - Op::InComma Right to Left
	else if (FourthPass(tokens)) 
	{
		return true;
	}

	return false;
}

bool FirstPass(std::vector<Token>* tokens)
{
	bool match = false;
	for (auto i = tokens->begin(); i+2 != tokens->end(); i++)
	{
		auto j = i + 1;
		if (i->type != Token::Expression || j->type != Token::Atom)
			continue;

		auto k = j + 1;

		if (k->type == Token::Atom)
		{
			// Expression Atom Atom
			// Left::Right
			// Left++
			// Left--
			// Left()

			if (j->rule != k->rule)
			{
				if (j->rule == Atom::LParen && k->rule == Atom::RParen)
				{
					//  Left()
					match = true;
					Expression e;
					e.rule = Op::FunctionCall;
					auto v = std::vector<Expression>();
					v.push_back(std::get<Expression>(i->value));
					e.value = v;

					i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
					tokens->erase(i+1, i+1+3);
				}
				continue;
			}

			// j->rule == k->rule

			auto l = k + 1;
			if (l != tokens->end() && l->type == Token::Expression &&
				j->rule == Atom::Colon)
			{
				// Left::Right
				match = true;
				Expression e;
				e.rule = Op::InNamespace;
				auto v = std::vector<Expression>();
				v.push_back(std::get<Expression>(i->value));
				v.push_back(std::get<Expression>(l->value));
				e.value = v;

				i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
				tokens->erase(i+1, i+1+4);
				continue;
			}

			if (j->rule == Atom::Plus)
			{
				// Left++
				match = true;
				Expression e;
				e.rule = Op::PostIncrement;
				auto v = std::vector<Expression>();
				v.push_back(std::get<Expression>(i->value));
				e.value = v;

				i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
				tokens->erase(i+1, i+1+3);
				continue;
			}

			if (j->rule == Atom::Minus)
			{
				// Left--
				match = true;
				Expression e;
				e.rule = Op::PostDecrement;
				auto v = std::vector<Expression>();
				v.push_back(std::get<Expression>(i->value));
				e.value = v;

				i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
				tokens->erase(i+1, i+1+3);
				continue;
			}
		}
		else if (k->type == EXPR)
		{
			// Expression Atom Expression
			// Left(Right)
			// Left[Right]
			// Left.Right

			auto l = k + 1;
			if (l != tokens->end() && l->type == Token::Atom)
			{
				if (j->rule == Atom::LParen && l->rule == Atom::RParen)
				{
					// Left(Right)
					match = true;
					Expression e;
					e.rule = Op::FunctionCall;
					auto v = std::vector<Expression>();
					v.push_back(std::get<Expression>(i->value));
					v.push_back(std::get<Expression>(k->value));
					e.value = v;

					i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
					tokens->erase(i+1, i+1+4);
					continue;
				}
				else if (j->rule == Atom::LBracket 
					&& l->rule == Atom::Exclamation + Atom::operators.find("]"))
				{
					// Left[Right]
					match = true;
					Expression e;
					e.rule = Op::ArrayIndex;
					auto v = std::vector<Expression>();
					v.push_back(std::get<Expression>(i->value));
					v.push_back(std::get<Expression>(k->value));
					e.value = v;

					i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
					tokens->erase(i+1, i+1+4);
					continue;
				}
			}

			if (j->rule == Atom::Dot)
			{
				// Left.Right
				match = true;
				Expression e;
				e.rule = Op::InDot;
				auto v = std::vector<Expression>();
				v.push_back(std::get<Expression>(i->value));
				v.push_back(std::get<Expression>(k->value));
				e.value = v;

				i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
				tokens->erase(i+1, i+1+3);
				continue;
			}
		}
	}

	return match;
}

bool SecondPass(std::vector<Token>* tokens)
{
	bool match = false;
	for (auto i = tokens->end() - 2; i + 1 != tokens->begin(); i--)
	{
		if (i->type != Token::Atom)
			continue;

		auto j = i + 1;
		auto right = j + 1;
		if (j->type == Token::Atom && 
			right < tokens->end() && right->type == Token::Expression)
		{
			if (i->rule == Atom::Plus && j->rule == Atom::Plus)
			{
				// ++Right
				match = true;
				Expression e;
				e.rule = Op::PreIncrement;
				auto v = std::vector<Expression>();
				v.push_back(std::get<Expression>(right->value));
				e.value = v;

				i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
				tokens->erase(i+1, i+1+3);
				continue;
			}

			if (i->rule == Atom::Minus && j->rule == Atom::Minus)
			{
				// --Right
				match = true;
				Expression e;
				e.rule = Op::PreDecrement;
				auto v = std::vector<Expression>();
				v.push_back(std::get<Expression>(right->value));
				e.value = v;

				i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
				tokens->erase(i+1, i+1+3);
				continue;
			}
		}
		else if (j->type == Token::Expression)
		{
			std::map<int, Op> rules;

			rules[Atom::Plus]        = Op::PrePlus;
			rules[Atom::Minus]       = Op::PreSubtract;
			rules[Atom::Exclamation] = Op::PreNot;
			rules[Atom::Tild]        = Op::PreBitNot;
			rules[Atom::Hash]        = Op::PreDereference;
			rules[Atom::At]          = Op::PreReference;

			auto op = rules.find(i->rule);
			if (op != rules.end())
			{
				// _Right
				match = true;
				Expression e;
				e.rule = op->second;
				auto v = std::vector<Expression>();
				v.push_back(std::get<Expression>(j->value));
				e.value = v;

				i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
				tokens->erase(i+1, i+1+2);
				continue;
			}
			if (i->rule == Atom::LBracket && i + 4 != tokens->end())
			{
				auto k = j + 1;
				auto right = k + 1;
				//if (k->rule == Atom::RBracket && )
			}
		}
	}
}