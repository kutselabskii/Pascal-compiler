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

	//TODO Default parameters should be changed
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
			break;

		case Types::float_type:
			value = std::stod(orig);
			break;

		case Types::string_type:
		case Types::id_type:
		case Types::char_type:
			value = const_cast<char*> (orig.c_str());
			break;

		case Types::separator_type:
			find_or_raise<Separators>(&value, &type, separators_list, orig);
			break;

		case Types::operator_type:
			find_or_raise<Operators>(&value, &type, operators_list, orig);
			break;

		case Types::end_of_file:
			value = "EOF";
			break;
		}
	}

	//TODO Think about name
	//TODO Need to check if operator == separator or conversely
	template <typename T> void find_or_raise (Value *v, Types *t, std::map<std::string, T> list, std::string o)
	{
		if (list.count(o))
			*v = list.find(o)->second;	//[o] doesn't working and I don't know why
		else
		{
			*t = unknown_character;
			*v = "unknown";
		}
	}
};



