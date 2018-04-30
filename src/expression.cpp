#include "operator.hpp"
#include "token.hpp"

void output_op(std::ostream& o, std::vector<Expression>& v, Op rule);

std::ostream& operator<<(std::ostream& o, Expression& e)
{
	if (e.rule != -1)
	{
		//output_op(o, std::get<std::vector<Expression>>(e.value), e.rule);
		o << "Operation: " << (int)e.rule;
		for (auto ex: std::get<std::vector<Expression>>(e.value))
			o << " " << ex;
	}
	else
	{
		o << " " << std::get<std::string>(e.value) << " "; 
	}

	return o;
}