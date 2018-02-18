#pragma once

#include <iostream>
#include <exception>
#include <string>

class LexerError : public std::exception
{
public:
	int line, col;
	char *text;

	LexerError(char *t, int l, int c)
	{
		text = t;
		line = l;
		col = c;
	}

	std::string what()
	{
		return ("Line " + std::to_string(line) + " : Col " + std::to_string(col) + " : " + text);
	}
};
	