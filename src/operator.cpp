#include "operator.hpp"
#include "atom.hpp"

#include <map>

#define EXPR (Token::Expression)
#define OP (Expression::Operation)

/// This file is a mess... But it works!
/// Manages the parsing of operation expressions

/// FirstPass manages all postfix and two infix operations
bool FirstPass(std::vector<Token>* tokens);

/// SecondPass manages all prefix operations
bool SecondPass(std::vector<Token>* tokens);

/// ThirdPass manages the rest of the infix inline operations
bool ThirdPass(std::vector<Token>* tokens);

/// FourthPass manages all assignment operations
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
					//&& l->rule == Atom::RBracket)
					&& l->rule == Atom::LBracket + 2)
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
			if (i->rule == Atom::LParen && i + 4 != tokens->end())
			{
				auto k = j + 1;
				auto right = k + 1;
				if (k->type == Token::Atom && k->rule == Atom::RParen
					&& right->type == Token::Expression)
				{
					// (j)Right
					match = true;
					Expression e;
					e.rule = Op::TypeCast;
					auto v = std::vector<Expression>();
					v.push_back(std::get<Expression>(j->value));
					v.push_back(std::get<Expression>(right->value));
					e.value = v;

					i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
					tokens->erase(i+1, i+1+4);
					continue;
				}
			}

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
			
		}
		else if (j->type == Token::Atom 
			&& j->rule >= Atom::Int && j->rule <= Atom::String)
		{
			if (i->rule == Atom::LParen && i + 4 != tokens->end())
			{
				auto k = j + 1;
				auto right = k + 1;
				if (k->type == Token::Atom && k->rule == Atom::RParen
					&& right->type == Token::Expression)
				{
					// (j)Right
					match = true;
					Expression e;
					e.rule = Op::TypeCast;
					auto v = std::vector<Expression>();
					v.push_back(std::get<Expression>(j->value));
					v.push_back(std::get<Expression>(right->value));
					e.value = v;

					i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
					tokens->erase(i+1, i+1+4);
					continue;
				}
			}
		}
	}
}

bool ThirdPass(std::vector<Token>* tokens)
{
	bool match = false;

	for (auto i = tokens->begin(); i + 2 != tokens->end(); i++)
	{
		auto j = i + 1;

		if (i->type == Token::Expression && j->type == Token::Expression)
		{
			if (j->rule == OP) 
			{
				auto ej = std::get<Expression>(j->value);
				if (ej.rule == Op::PrePlus)
				{
					// i+j
					match = true;
					Expression e;
					e.rule = Op::InAdd;
					auto v = std::vector<Expression>();
					v.push_back(std::get<Expression>(i->value));
					v.push_back(ej);
					e.value = v;

					i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
					tokens->erase(i+1, i+1+3);
				}
				else if (ej.rule == Op::PreSubtract)
				{
					// i-j
					match = true;
					Expression e;
					e.rule = Op::InSubtract;
					auto v = std::vector<Expression>();
					v.push_back(std::get<Expression>(i->value));
					v.push_back(ej);
					e.value = v;

					i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
					tokens->erase(i+1, i+1+3);
				}
			}
			continue;
		}

		if (i->type != Token::Expression || j->type != Token::Atom)
			continue;

		auto k = j + 1;
		if (k->type == Token::Expression)
		{
			std::map<int, Op> rules;

			rules[Atom::Star]         = Op::InMultiply;
			rules[Atom::ForwardSlash] = Op::InDivide;
			rules[Atom::Percent]      = Op::InModulo;
			rules[Atom::Plus]         = Op::InAdd;
			rules[Atom::Minus]        = Op::InSubtract;
			rules[Atom::Less]         = Op::InLessThan;
			rules[Atom::Greater]      = Op::InGreaterThan;
			rules[Atom::Ampersand]    = Op::InBitAnd;
			rules[Atom::Carat]        = Op::InBitXor;
			rules[Atom::Vertical]     = Op::InBitOr;
			
			auto op = rules.find(i->rule);
			if (op != rules.end())
			{
				// i_k
				match = true;
				Expression e;
				e.rule = op->second;
				auto v = std::vector<Expression>();
				v.push_back(std::get<Expression>(i->value));
				v.push_back(std::get<Expression>(k->value));
				e.value = v;

				i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
				tokens->erase(i+1, i+1+3);
				continue;
			}

		} 
		else if (i + 3 != tokens->end()) 
		{
			auto l = k + 1;

			if (k->type == Token::Atom && l->type == Token::Expression)
			{
				std::map<std::pair<int, int>, Op> rules;

				rules[std::make_pair(Atom::Less, Atom::Less)]           = Op::InShiftLeft;
				rules[std::make_pair(Atom::Greater, Atom::Greater)]     = Op::InShiftRight;
				rules[std::make_pair(Atom::Less, Atom::Equal)]          = Op::InLessThanEq;
				rules[std::make_pair(Atom::Greater, Atom::Equal)]       = Op::InGreaterThanEq;
				rules[std::make_pair(Atom::Equal, Atom::Equal)]         = Op::InEqualTo;
				rules[std::make_pair(Atom::Exclamation, Atom::Equal)]   = Op::InNotEqualTo;
				rules[std::make_pair(Atom::Ampersand, Atom::Ampersand)] = Op::InAnd;
				rules[std::make_pair(Atom::Vertical, Atom::Vertical)]   = Op::InOr;

				auto op = rules.find(std::make_pair(j->rule, k->rule));
				if (op != rules.end())
				{
					// i__l
					match = true;
					Expression e;
					e.rule = op->second;
					auto v = std::vector<Expression>();
					v.push_back(std::get<Expression>(i->value));
					v.push_back(std::get<Expression>(l->value));
					e.value = v;

					i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
					tokens->erase(i+1, i+1+3);
					continue;
				}
			}
		}
	}

	return match;
}

bool FourthPass(std::vector<Token>* tokens) 
{
	bool match = false;

	for (auto i = tokens->end() - 3; i + 1 != tokens->begin(); i--)
	{
		auto j = i + 1;

		if (i->type != Token::Expression || j->type != Token::Atom)
			continue;

		auto k = j + 1;

		if (k->type == Token::Expression)
		{
			if (j->rule == Atom::Equal)
			{
				// i=k
				match = true;
				Expression e;
				e.rule = Op::InAssign;
				auto v = std::vector<Expression>();
				v.push_back(std::get<Expression>(i->value));
				v.push_back(std::get<Expression>(k->value));
				e.value = v;

				i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
				tokens->erase(i+1, i+1+3);
			}
			else if (j->rule == Atom::Comma)
			{
				// i,k
				match = true;
				Expression e;
				e.rule = Op::InComma;
				auto v = std::vector<Expression>();
				v.push_back(std::get<Expression>(i->value));

				auto ek = std::get<Expression>(k->value);
				if (ek.rule == Op::InComma)
				{
					auto vk = std::get<std::vector<Expression>>(ek.value);
					v.insert(v.end(), vk.begin(), vk.end());
				}
				else
				{
					v.push_back(ek);
				}

				e.value = v;

				i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
				tokens->erase(i+1, i+1+3);
			}
			continue;
		}

		auto l = k + 1;

		if (k->type != Token::Atom || l == tokens->end())
			continue;

		if (l->type == Token::Expression
			&& k->rule == Atom::Equal)
		{
			// i_=l
			Op rule;

			switch (j->rule)
			{
			case Atom::Plus:
				rule = Op::InSumAssign;
				break;
			case Atom::Minus:
				rule = Op::InSubtractAssign;
				break;
			case Atom::Star:
				rule = Op::InMultiplyAssign;
				break;
			case Atom::ForwardSlash:
				rule = Op::InDivideAssign;
				break;
			case Atom::Percent:
				rule = Op::InModuloAssign;
				break;
			case Atom::Ampersand:
				rule = Op::InBitAndAssign;
				break;
			case Atom::Carat:
				rule = Op::InBitXorAssign;
				break;
			case Atom::Vertical:
				rule = Op::InBitOrAssign;
				break;
			default:
				continue;
			}

			match = true;
			Expression e;
			e.rule = rule;
			auto v = std::vector<Expression>();
			v.push_back(std::get<Expression>(i->value));
			v.push_back(std::get<Expression>(l->value));
			e.value = v;

			i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
			tokens->erase(i+1, i+1+4);

			continue;
		}

		auto m = l + 1;

		if (l->type != Token::Atom || m == tokens->end())
			continue;

		if (m->type == Token::Expression
			&& l->rule == Atom::Equal)
		{
			Op rule;
			if (j->rule == Atom::Less && k->rule == Atom::Less)
				rule = Op::InShiftLeftAssign;
			else if (j->rule == Atom::Greater && k->rule == Atom::Greater)
				rule = Op::InShiftRightAssign;
			else
				continue;

			match = true;
			Expression e;
			e.rule = rule;
			auto v = std::vector<Expression>();
			v.push_back(std::get<Expression>(i->value));
			v.push_back(std::get<Expression>(m->value));
			e.value = v;

			i = tokens->insert(i, Token(i->loc, EXPR, OP, e));
			tokens->erase(i+1, i+1+5);
		}
	}

	return match;
}