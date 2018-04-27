#include <iostream>
#include <iomanip>

#include "parser.h"
#include "generator.h"

class Asm_code;
std::shared_ptr<Asm_code> Generate_code(std::shared_ptr<_nodes::Node>);
bool generate(std::shared_ptr<_nodes::Node> node);
int get_offset(std::shared_ptr<Symbol_table> symbol_table, std::shared_ptr<_nodes::Node> node);

template<typename T> void print_typevalue(const char *type, T value)
{
	std::cout << std::setw(15) << std::left << type;
	std::cout << std::setw(30) << std::left << value;
}

void lexer_test(Lexer *lexer)
{
	Token t;

	std::cout << std::setw(5) << std::left << "Line";
	std::cout << std::setw(5) << std::left << "Col";
	std::cout << std::setw(15) << std::left << "Type";
	std::cout << std::setw(30) << std::left << "Value";
	std::cout << std::setw(30) << std::left << "Original" << std::endl;

	do
	{
		try
		{
			t = lexer->next();

			std::cout << std::setw(5) << std::left << t.pos_line;
			std::cout << std::setw(5) << std::left << t.pos_col;
			switch (t.type)
			{
			case Types::int_type:
				print_typevalue("Int", int(t.value));
				break;

			case Types::float_type:
				print_typevalue("Float", double(t.value));
				break;

			case Types::id_type:
				print_typevalue("Id", std::string(t.value));
				break;

			case Types::string_type:
				print_typevalue("String", std::string(t.value));
				break;

			case Types::char_type:
				print_typevalue("Char", std::string(t.value));
				break;

			case Types::keyword_type:
				print_typevalue("Keyword", keywords_names.find(Keywords(t.value))->second);
				break;

			case Types::separator_type:
				print_typevalue("Separator", separators_names.find(Separators(t.value))->second);
				break;

			case Types::operator_type:
				print_typevalue("Operator", operators_names.find(Operators(t.value))->second);
				break;

			case Types::end_of_file:
				print_typevalue("End of file", "EOF");
				break;
			}
			std::cout << std::setw(30) << std::left << t.original << std::endl;

		}
		catch (LexerError& e)
		{
			std::cout << e.what() << std::endl;
			return;
		}
	} while (t.type != Types::end_of_file);
}

void expressions_test(Lexer *lexer)
{
	Parser parser(lexer);

	parser.expressions_test();
	parser.print();
}

void parser_test(Lexer *lexer)
{
	Parser parser(lexer);

	parser.parse();
	parser.print();
}

void generator_test(Lexer *lexer, std::string filename)
{
	try
	{
		filename.pop_back();
		filename.pop_back();
		filename += "asm";

		Parser parser(lexer);

		parser.parse();
		std::shared_ptr<Asm_code> code = Generate_code(parser.get_root());
		code->print(filename);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

int main(int argc, char **argv)
{
	std::string filename;
	int mode;
	Lexer *lexer;
	switch (argc)
	{
	case 1:
		//filename = "D:/Homework/Compiler/tests/lexer/32.in";	mode = 1;
		//filename = "D:/Homework/Compiler/tests/expressions/7.in";	mode = 2;
		//filename = "D:/Homework/Compiler/tests/parser/34.in";	mode = 3;
		//filename = "D:/Homework/Compiler/tests/semantic/28.in";	mode = 3;
		filename = "D:/Homework/Compiler/tests/generator/25.in"; mode = 3;
		break;
	case 3:
		filename = argv[1];
		mode = atoi(argv[2]);
		break;
	default:
		std::cout << "Incorrect command line arguments" << std::endl;
		return -1;
	}

	lexer = new Lexer(filename);
	switch (mode)
	{
	case 1:
		lexer_test(lexer);
		break;
	case 2:
		expressions_test(lexer);
		break;
	case 3:
		parser_test(lexer);
		break;
	case 4:
		generator_test(lexer, filename);
		break;
	}

	if (argc == 1)
		system("pause");
	return 0;
}