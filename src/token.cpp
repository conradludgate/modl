#include "atom.hpp"

std::ostream& operator<<(std::ostream& o, Token& t)
{
	switch(t.type)
	{
	case Token::Expression:
		o << std::get<Expression>(t.value);
		break;
	case Token::Include:
		std::get<IncludeClass>(t.value).output(o, t.rule);
		break;
	case Token::Atom:
		if (t.rule >= Atom::keywords.size())
		{
			o << "Operator: " << t.rule << " " << Atom::operators[t.rule - Atom::keywords.size()];
		}
		else
		{
			o << Atom::keywords[t.rule];
		}
		break;
	}

	return o;
}