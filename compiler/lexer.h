#pragma once

#include <fstream>
#include <string>

#include "enums.h"
#include "token.h"
#include "exceptions.h"
#include "table.h"

class Lexer
{
public:

	Lexer();
	Lexer(std::string);
	~Lexer();

	Token next();


private:
	std::ifstream file_;
	States state_;

	int pos_line;
	int pos_col;

	std::string current_;
	Token last_;

	//TODO Think about name
	Token make_or_throw(Types);

};