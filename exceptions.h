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

class ParserError : public std::exception
{
public:
	int line, col;
	char *text;

	ParserError(char *t, int l, int c)
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

class SemanticError : public std::exception
{
public:
	int line, col;
	char *text;

	SemanticError(char *t, int l, int c)
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
	