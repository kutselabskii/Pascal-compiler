#pragma once

#include <string>

#include "enums.h"



class Token
{
public:

	union Value
	{
		unsigned long int_value;
		double float_value;
		char* string_value;

		operator int() const { return int_value; }
		operator double() const { return float_value; }
		operator std::string() const { return std::string(string_value); }

		unsigned long operator= (const unsigned long &other) { int_value = other; return int_value; }
		unsigned long operator= (const int &other) { int_value = other; return int_value; }
		double operator= (const double &other) { float_value = other; return float_value; }
		std::string operator= (char *other) { string_value = other; return std::string(string_value); }
	};

	Types type;
	int pos_line, pos_col;
	std::string original;
	Value value;

	Token() { }
	~Token() { }

	//DEFAULT PARAMETERS SHOULD BE CHANGED
	Token(Types t, int line = 0, int col = 0, std::string orig = " ")
	{
		type = t;
		pos_line = line;
		pos_col = col;
		original = orig;

		switch (type)
		{
		case Types::int_type:
			value = std::stoi(orig);

		case Types::float_type:
			value = std::stod(orig);

		case Types::string_type:
		case Types::id_type:
			value = const_cast<char*> (orig.c_str());

		case Types::operator_type:
			if (operators_list.count(orig))
				value = operators_list.find(orig)->second;	//[orig] doesn't working and I don't know why
			else
			{
				type = unknown_character;
				value = "unknown";
			}

		case Types::end_of_file:
			value = "EOF";
		}
	}
};

