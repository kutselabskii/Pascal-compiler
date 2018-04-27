#pragma once

#include "parser.h"

class Asm_code;

const std::shared_ptr<Asm_code> asm_code = std::make_shared<Asm_code>();

class Asm_element
{
public:
	Asm_element(Asm_commands);

	virtual std::string print();

	Asm_commands op_code;
};

class Asm_label : public Asm_element
{
	Asm_label(Asm_commands op_code) : Asm_element(op_code) {}
	std::string print();
};

class Asm_constant
{
public:
	Asm_constant(std::string, std::string, std::string);
	std::string print();
	
	std::string name, dx, content;
};

class Asm_code
{
public:
	void add_command(Asm_commands);
	void add_command(Asm_commands, int);
	void add_command(Asm_commands, Registers);
	void add_command(Asm_commands, Registers, int);
	void add_command(Asm_commands, int, int);
	void add_command(Asm_commands, Registers, Registers);
	void add_command(std::string);

	//void add_command(std::shared_ptr<Asm_element>);
	void add_constant(std::shared_ptr<Asm_constant>);
	void print(std::string filename);
	
	std::vector<std::string> commands;
	std::vector<std::shared_ptr<Asm_constant>> constants;
	std::vector<std::shared_ptr<Symbol_table>> symbol_table_stack;

	int labelcount = 0;
};


std::shared_ptr<Asm_code> Generate_code(std::shared_ptr<_nodes::Node> node);

bool generate(std::shared_ptr<_nodes::Node> node);

int get_offset(std::shared_ptr<Symbol_table> symbol_table, std::shared_ptr<_nodes::Node> node);
int get_max_offset(std::shared_ptr<Symbol_table> symbol_table);
