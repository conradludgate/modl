#include "atom.hpp"
#include <limits>

std::string MatchKeywords(std::istream& is);
bool MatchWS(std::istream& is);
bool MatchComment(std::istream& is);
std::string MatchNumLiteral(std::istream& is);
std::string MatchStringLiteral(std::istream& is);

std::vector<Token> Atom::Atomise(std::istream& is)
{
	std::vector<Token> tokens;

	while(!is.fail())
	{
		auto p = is.tellg();

		auto data = MatchKeywords(is);
		if (data.size() != 0)
		{
			bool match = false;
			for (int i = 0; i < Atom::keywords.size(); ++i)
			{
				if (Atom::keywords[i] == data)
				{
					match = true;
					tokens.push_back(Token(p, Token::Atom, i));
					break;
				}
			}

			if (!match) 
			{
				Expression e;
				e.value = data;

				tokens.push_back(
					Token(p, Token::Expression, Expression::Identifier, e));
			}
		} 
		else if (MatchWS(is) || MatchComment(is))
		{
			continue;
		} 
		else if (data = MatchNumLiteral(is); data.size() != 0)
		{
			Expression e;
			e.value = data;

			tokens.push_back(
				Token(p, Token::Expression, Expression::NumberLiteral, e));
		}
		else if (data = MatchStringLiteral(is); data.size() != 0)
		{
			Expression e;
			e.value = data;
			
			tokens.push_back(
				Token(p, Token::Expression, Expression::StringLiteral, e));
		}
		else
		{
			char c;
			is.get(c);

			int op = Atom::operators.find(c);

			if (op < 0)
			{
				std::cout << "UNDEFINED CHARACTER " << c << std::endl;
			}
			else
			{
				auto token = Token(p, Token::Atom, Atom::Exclamation + op);
				tokens.push_back(token);
			}
		}

	}

	return tokens;
}

std::string MatchKeywords(std::istream& is)
{
	std::string data;

	char c;
	is.get(c);
	if (!isalpha(c) && c != '_')
	{
		is.putback(c);
		return "";
	}
	
	do 
	{ 
		data += c; 
		is.get(c); 
	} while (isalnum(c) || c == '_');

	is.putback(c);
	return data;
}

bool MatchWS(std::istream& is)
{
	if (!isspace(is.peek()))
		return false;
	
	do { is.ignore(); } while (isspace(is.peek()));
	return true;
}

bool MatchComment(std::istream& is) {
	if (is.peek() == '/')
	{
		is.ignore();

		if (is.peek() == '/') 
		{
			is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return true;
		}
		else if (is.peek() == '*')
		{
			is.ignore();
			is.ignore(std::numeric_limits<std::streamsize>::max(), '*');
			while (is.peek() != '/') 
			{
				is.ignore(std::numeric_limits<std::streamsize>::max(), '*');
			}
			is.ignore();
			return true;
		} 
		else 
		{
			is.putback('/');
			return false;
		}
	}

	return false;
}

std::string MatchNumLiteral(std::istream& is)
{
	std::string data = "";

	char c = is.peek();

	if (!isdigit(c))
	{
		return "";
	}

	int base = 10;

	is.ignore();

	if (c == '0')
	{	
		char d;
		is.get(d); // d is the character after c
		if (isdigit(d)) // 01
		{
			data += d;		
		}
		else if (d == 'x' || d == 'X') // 0x
		{
			//data[0] = '0' + 16; // Hex
			base = 16;
			data += "0x";
		}
		else if (d == 'b' || d == 'B') // 0b
		{
			//data[0] = '0' + 2; // Binary
			base = 2;
			data += "0b";
		} 
		else
		{
			data += c;
			if (d == '.') // 0.
			{
				//data[0] = '0'; // Float
				base = 0;
				data += '.';
			}
			else // 0_
			{
				is.putback(d);
				return data;
			}
		}
	} 
	else
	{
		data += c;
	}

	is.get(c);

	while (isdigit(c))
	{
		data += c;
		is.get(c);
	}

	if (base == 10)
	{
		if (c == '.' || c == 'E' || c == 'e') // 12., 12e, 12E
		{
			//data[0] = '0'; // Float
			if (c == 'E' || c == 'e')
			{
				is.get(c);
				if (c == '-') // 12e-, 12E-
				{
					data += c;
					is.get(c);
				}
				if (c == '+') // 12e+, 12E+
				{
					is.get(c);
				}
			}
			else
			{
				data += c;
			}

			while (isdigit(c))
			{
				data += c;
				is.get(c);
			}
		}
	}

	is.putback(c);

	if (data[data.size()-1] == '-') // 12e-_
	{
		is.putback('-');
		data.pop_back();
	}
	return data;
}

std::string MatchStringLiteral(std::istream& is)
{
	std::string data = "";

	//int pos = is.tellg();

	char c = is.peek();
	if (c != '\"')
	{
		return data;
	}
	
	is.ignore();
	while (true)
	{
		is.get(c);
		if (is.fail())
		{
			//is.seekg(pos, is.beg);
			return data;
		}

		bool escaped = false;
		switch (c) 
		{
		case '"':
			if (escaped) {
				data += '\"';
				break;
			}
			return data;

		case '\\':
			if (escaped) {
				data += '\\';
			}
			escaped = !escaped;
			break;
		case 'n':
			if (escaped) {
				data += '\n';
			} else {
				data += 'n';
			}
			break;
		case 'r':
			if (escaped) {
				data += '\r';
			} else {
				data += 'r';
			}
			break;
		case 't':
			if (escaped) {
				data += '\t';
			} else {
				data += 't';
			}
			break;
		default:
			data += c;
			break;
		} 
	}

	return data;
}