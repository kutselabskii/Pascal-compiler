#include <iostream>
#include <iomanip>

#include "lexer.h"

int main(int argc, char **argv)
{
	Lexer lexer("../test/1.in");
	Token t;

	std::cout << std::setw(5) << std::left << "Line";
	std::cout << std::setw(5) << std::left << "Col";
	std::cout << std::setw(10) << std::left << "Type";
	std::cout << std::setw(30) << std::left << "Value";
	std::cout << std::setw(30) << std::left << "Original" << std::endl;

	do
	{
		try
		{
			t = lexer.next();


			std::cout << std::setw(5) << std::left << t.pos_line;
			std::cout << std::setw(5) << std::left << t.pos_col;
			switch (t.type)
			{
			case Types::int_type:
				std::cout << std::setw(10) << std::left << "Int";
				std::cout << std::setw(30) << std::left << int(t.value);
			
			case Types::float_type:
				std::cout << std::setw(10) << std::left << "Float";
				std::cout << std::setw(30) << std::left << double(t.value);

			case Types::string_type:
				std::cout << std::setw(10) << std::left << "String";
				std::cout << std::setw(30) << std::left << std::string(t.value);

			case Types::end_of_file:
				std::cout << std::setw(10) << std::left << "End of file";
				std::cout << std::setw(30) << std::left << std::string(t.value);
			}
			std::cout << std::setw(30) << std::left << t.original << std::endl;
			
		}
		catch (LexerError& e)
		{
			std::cout << e.what() << std::endl;
			system("pause");
			return -1;
		}
	} while (t.type != Types::end_of_file);

	system("pause");
	return 0;
}