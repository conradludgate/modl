#include <fstream>
#include "compiler.hpp"

int main(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		std::ifstream ifs(argv[i], std::ifstream::in);

		auto tokens = Compiler::Compile(ifs);

		std::string filename(argv[i]);
		filename += ".out";
		std::ofstream ofs(filename.c_str(), std::ofstream::out);

		for (auto token: tokens)
		{
			ofs << token << std::endl;
		}
	}
}