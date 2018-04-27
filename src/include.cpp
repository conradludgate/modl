#include "token.hpp"
#include "atom.hpp"

bool IncludeClass::Match(std::vector<Token>* tokens) {
	if (tokens->size() < 3) {
		return false;
	}

	bool match = false;
	for (auto i = tokens->begin(); i+2 != tokens->end(); i++)
	{
		if (i->type != Token::Atom) continue;

		int rule = IncludeClass::Module;
		if (i->rule == Atom::Include) rule = IncludeClass::Include;
		else if (i->rule != Atom::Module) continue;

		if ((i+1)->type != Token::Expression || 
			(i+1)->rule != Expression::StringLiteral)
		{
			// Error
			continue;
		}

		if ((i+2)->type != Token::Atom || 
			(i+2)->rule != Atom::Semicolon)
		{
			// Error
			continue;
		}

		match = true;
		IncludeClass in;
		in.path = std::get<0>(
			std::get<1>((i+1)->value)
			.value);

		i = tokens->insert(i, Token(i->loc, Token::Include, rule, in));
		tokens->erase(i+1, i+4);
	}

	return match;
}