#include "atom.hpp"
#include <functional>

typedef std::function<bool(std::vector<Token>*)> Matcher;
//typedef bool (*TokenMatcher) (std::vector<Token>* tokens);

class Compiler
{
public:
	inline static std::vector<Matcher> matchers;

	static void Init()
	{
		matchers.push_back(&IncludeClass::Match);
		matchers.push_back(&Expression::MatchOperator);
	}

	static std::vector<Token> Compile(std::istream& is)
	{
		auto tokens = Atom::Atomise(is);

		if (matchers.size() == 0)
			Init();

		for (int i = 0; i < matchers.size();)
		{
			if (matchers[i](&tokens))
			{
				i = 0;
			}
			else 
			{
				i++;
			}
		}

		return tokens;
	}
};