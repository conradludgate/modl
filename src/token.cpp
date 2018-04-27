#include "atom.hpp"

std::ostream& operator<<(std::ostream& o, Token& t)
{
	switch(t.type)
	{
	case Token::Expression:
		o << std::get<Expression>(t.value);
		break;
	case Token::Include:
		o << std::get<IncludeClass>(t.value);
		break;
	case Token::Atom:
		if (t.rule >= Atom::keywords.size())
		{
			o << Atom::operators[t.rule - Atom::keywords.size()];
		}
		else
		{
			o << Atom::keywords[t.rule];
		}
		break;
	}

	return o;
}