#include "operator.hpp"
#include "atom.hpp"

#define AP(X) std::make_pair(Token::Atom, Atom::X)
#define EP std::make_pair(Token::Expression, -1)
// #define EPP(X) std::make_pair(Token::Expression, Op::X)

bool FirstPass(std::vector<Token>* tokens);
bool SecondPass(std::vector<Token>* tokens);
bool ThirdPass(std::vector<Token>* tokens);
bool FourthPass(std::vector<Token>* tokens);

bool Expression::MatchOperator(std::vector<Token>* tokens)
{
	return tokens->size() >= 2
		&& FirstPass (tokens)
		|| SecondPass(tokens)
		|| ThirdPass (tokens)
		|| FourthPass(tokens);
}

bool match(int rule, std::vector<std::pair<int, int>> pattern, 
	std::vector<Token>::iterator start,
	std::vector<Token>* tokens)
{
	std::vector<Expression> exps;

	auto p = pattern.begin();
	auto end = start + pattern.size();
	for (auto i = start;
		p != pattern.end() || i != end;
		p++, i++)
	{
		if (p->first != i->type)
			return false;

		if (p->second >= 0 && p->second != i->rule)
			return false;

		if (p->first == Token::Expression)
			exps.push_back(std::get<Expression>(i->value));
	}

	if (p != pattern.end())
		return false;

	Expression e;
	e.rule = rule;
	e.value = exps;

	start = tokens->insert(start, Token(start->loc, 
		Token::Expression, Expression::Operation, e));
	tokens->erase(start+1, start+1+pattern.size());
}

bool FirstPass(std::vector<Token>* tokens)
{
	for (auto i = tokens->begin(); i != tokens->end(); i++)
	{
		if (match(Op::InNamespace, {EP, AP(Colon), AP(Colon), EP}, i, tokens) ||
			match(Op::PostIncrement, {EP, AP(Plus), AP(Plus)}, i, tokens) ||
			match(Op::PostDecrement, {EP, AP(Minus), AP(Minus)}, i, tokens) ||
			match(Op::FunctionCall, {EP, AP(LParen), AP(RParen)}, i, tokens) ||
			match(Op::FunctionCall, {EP, AP(LParen), EP, AP(RParen)}, i, tokens) ||
			match(Op::Parenthesis, {AP(LParen), EP, AP(RParen)}, i, tokens) ||
			match(Op::ArrayIndex, {EP, AP(LBracket), EP, AP(LBracket+2)}, i, tokens) ||
			match(Op::InDot, {EP, AP(Dot), EP}, i, tokens))
			return true;
	}

	return false;
}

bool SecondPass(std::vector<Token>* tokens)
{
	for (auto i = tokens->end() - 2; i + 1 != tokens->begin(); i--)
	{
		if (//match(Op::PreIncrement, {AP(Plus), EPP(PrePlus)}, i, tokens) ||
			//match(Op::PreDecrement, {AP(Minus), EPP(PreSubtract)}, i, tokens) ||
			match(Op::PrePlus, {AP(Plus), EP}, i, tokens) ||
			match(Op::PreSubtract, {AP(Minus), EP}, i, tokens) ||
			match(Op::PreNot, {AP(Exclamation), EP}, i, tokens) ||
			match(Op::PreBitNot, {AP(Tild), EP}, i, tokens) ||
			match(Op::TypeCast, {AP(LBrace), EP, AP(RBrace), EP}, i, tokens) ||
			match(Op::PreDereference, {AP(Hash), EP}, i, tokens) ||
			match(Op::PreReference, {AP(At), EP}, i, tokens))
			return true;
	}

	return false;
}

bool ThirdPass(std::vector<Token>* tokens)
{
	for (auto i = tokens->begin(); i + 2 != tokens->end(); i++)
	{
		if (match(Op::InMultiply, {EP, AP(Star), EP}, i, tokens) ||
			match(Op::InDivide, {EP, AP(ForwardSlash), EP}, i, tokens) ||
			match(Op::InModulo, {EP, AP(Percent), EP}, i, tokens) ||
			//match(Op::InAdd, {EP, AP(Plus), EP}, i, tokens) ||
			//match(Op::InSubtract, {EP, AP(Minus), EP}, i, tokens) ||
			match(Op::InShiftLeft, {EP, AP(Less), AP(Less), EP}, i, tokens) ||
			match(Op::InShiftRight, {EP, AP(Greater), AP(Greater), EP}, i, tokens) ||
			match(Op::InLessThan, {EP, AP(Less), EP}, i, tokens) ||
			match(Op::InLessThanEq, {EP, AP(Less), AP(Equal), EP}, i, tokens) ||
			match(Op::InGreaterThan, {EP, AP(Greater), EP}, i, tokens) ||
			match(Op::InGreaterThanEq, {EP, AP(Greater), AP(Equal), EP}, i, tokens) ||
			match(Op::InEqualTo, {EP, AP(Equal), AP(Equal), EP}, i, tokens) ||
			match(Op::InNotEqualTo, {EP, AP(Exclamation), AP(Equal), EP}, i, tokens) ||
			match(Op::InBitAnd, {EP, AP(Ampersand), EP}, i, tokens) ||
			match(Op::InBitXor, {EP, AP(Carat), EP}, i, tokens) ||
			match(Op::InBitOr, {EP, AP(Vertical), EP}, i, tokens) ||
			match(Op::InAnd, {EP, AP(Ampersand), AP(Ampersand), EP}, i, tokens) ||
			match(Op::InOr, {EP, AP(Vertical), AP(Vertical), EP}, i, tokens))
			return true;
	}

	return false;
}

bool FourthPass(std::vector<Token>* tokens) 
{
	for (auto i = tokens->end() - 3; i + 1 != tokens->begin(); i--)
	{
		if (match(Op::InAssign, {EP, AP(Equal), EP}, i, tokens) ||
			match(Op::InAddAssign, {EP, AP(Plus), AP(Equal), EP}, i, tokens) ||
			match(Op::InSubtractAssign, {EP, AP(Minus), AP(Equal), EP}, i, tokens) ||
			match(Op::InMultiplyAssign, {EP, AP(Star), AP(Equal), EP}, i, tokens) ||
			match(Op::InDivideAssign, {EP, AP(ForwardSlash), AP(Equal), EP}, i, tokens) ||
			match(Op::InModuloAssign, {EP, AP(Percent), AP(Equal), EP}, i, tokens) ||
			match(Op::InShiftLeftAssign, {EP, AP(Less), AP(Less), AP(Equal), EP}, i, tokens) ||
			match(Op::InShiftRightAssign, {EP, AP(Greater), AP(Greater), AP(Equal), EP}, i, tokens) ||
			match(Op::InBitAndAssign, {EP, AP(Ampersand), AP(Equal), EP}, i, tokens) ||
			match(Op::InBitXorAssign, {EP, AP(Carat), AP(Equal), EP}, i, tokens) ||
			match(Op::InBitOrAssign, {EP, AP(Vertical), AP(Equal), EP}, i, tokens))
			//match(Op::InComma, {EP, AP(Comma), EP}, i, tokens))
			return true;
	}

	return false;
}