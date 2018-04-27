#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <limits>

struct error {
	int pos;
	std::string message;
};
	
class Error 
{
public:
	Error(std::istream& is) : is(is) {}

	void PushError(int pos, std::string message)
	{
		for (auto i = messages.begin(); i < messages.end(); ++i)
		{
			//if (line < i->line || (line == i->line && col <= i->line))
			if(pos <= i->pos)
			{
				messages.insert(i, error{line, col, message});
				return;
			}
		}
		messages.push_back(error{line, col, message});
	}

	void Merge(Error& e)
	{
		auto j = e.messages.begin()
		for (auto i = messages.begin(); i < messages.end() || j < e.messages.end(); i++)
		{
			//if (j->line < i->line || (j->line == i->line && j->col <= i->line))
			if(j->pos <= i->pos)
			{
				messages.insert(i, *(j++));
			}
		}

		while (j < messages.end())
		{
			messages.push_back(*(j++));
		}
	}

	std::ostream& operator<<(std::ostream& o, Error& e)
	{
		int linen = 1; // Line number

		is.seekg(0, is.beg);
		int lastn = 0; // last newline

		auto max = std::numeric_limits<streamsize>::max();

		is.ignore(max, '\n');
		int nextn = is.tellg(); // next newline

		for (const auto& error: Error)
		{
			while (nextn < error.pos)
			{
				lastn = nextn;
				is.ignore(max, '\n');
				nextn = is.tellg();

				linen++;
			}

			is.seekg(lastn, is.beg);

			o << "Line " << linen << " Col " << error.pos - lastn + 1 << ":" << std::endl;

			std::string code_line;
			std::getline(is, code_line);
			o << "\t" << code_line << std::endl;

			o << "\t" << std::setfill(' ') << std::setw(error.pos - lastn) << "^" << std::endl;
			o << error.message << std::endl << std::endl;
		}

		return o;
	}

	// bool did_match()
	// {
	// 	return partial_match;
	// }

	bool bool()
	{
		return messages.size() == 0;
	}

	int errors()
	{
		return messages.size();
	}

private:
	std::istream& is;
	std::vector<error> messages;

	//bool partial_match = false;
};