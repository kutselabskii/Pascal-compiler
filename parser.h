#pragma once
#include <memory>

#include "lexer.h"
#include "node.h"
#include "symbol_table.h"


std::shared_ptr<_nodes::Node> operator+(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right);
std::shared_ptr<_nodes::Node> operator-(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right);
std::shared_ptr<_nodes::Node> operator/(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right);
std::shared_ptr<_nodes::Node> operator*(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right);
std::shared_ptr<_nodes::Node> operator_div(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right);
std::shared_ptr<_nodes::Node> operator_mod(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right);
std::shared_ptr<_nodes::Node> operator_unary_minus(std::shared_ptr<_nodes::Node> _left);
std::shared_ptr<_nodes::Node> get_type(std::shared_ptr<_nodes::Node> node);

class Parser
{
public:
	Parser(Lexer*);
	~Parser();

	void parse();

	void expressions_test();

	void print();

	std::shared_ptr<_nodes::Node> get_root()
	{
		return root;
	}

private:
	Lexer *lexer;
	std::shared_ptr<_nodes::Node> root;
	std::vector<std::shared_ptr<Symbol_table>> symbol_table_stack;
	std::shared_ptr<_nodes::Node> stack_search(Symbol_table::Field field, std::string key);
	bool id_is_duplicated(std::string id);



	//Programs and blocks
	std::shared_ptr<_nodes::Node> parse_program();
	std::shared_ptr<_nodes::Node> parse_program_heading();
	std::shared_ptr<_nodes::Node> parse_block();
	
	//Statement part
	std::shared_ptr<_nodes::Node> parse_statement_part();
	std::shared_ptr<_nodes::Node> parse_statement_sequence();	
	std::shared_ptr<_nodes::Node> parse_statement();

	//Declaration part
	std::shared_ptr<_nodes::Node> parse_declaration_part();
	std::shared_ptr<_nodes::Node> parse_constant_definition_part();
	std::shared_ptr<_nodes::Node> parse_variable_declaration_part();	//Got things to do
	std::shared_ptr<_nodes::Node> parse_type_definition_part();
	std::shared_ptr<_nodes::Node> parse_procedure_declaration();
	std::shared_ptr<_nodes::Node> parse_function_declaration();
	std::shared_ptr<_nodes::Node> parse_function_arguments_list();
	
	//Low-level
	std::vector<std::string> parse_identifier_list();	//Maybe need to rethink something

	//Types
	std::shared_ptr<_nodes::Node> parse_type();	//Complicated types not done yet
	
	//Statements
	std::shared_ptr<_nodes::Node> parse_assignment_statement();
	std::shared_ptr<_nodes::Node> parse_procedure_statement();
	std::shared_ptr<_nodes::Node> parse_compound_statement();
	std::shared_ptr<_nodes::Node> parse_while_statement();
	std::shared_ptr<_nodes::Node> parse_repeat_statement();
	std::shared_ptr<_nodes::Node> parse_for_statement();
	std::shared_ptr<_nodes::Node> parse_write_statement();
	std::shared_ptr<_nodes::Node> parse_if_statement();

	//Expressions
	std::shared_ptr<_nodes::Node > parse_expression(int mode = 0);
	std::shared_ptr<_nodes::Node> parse_factor();
	std::shared_ptr<_nodes::Node> parse_function_call();

	//Expressions WO check
	std::shared_ptr<_nodes::Node> parse_factor_WO_check();
	bool expressions_wo_check;

	void print_tree(std::shared_ptr<_nodes::Node>, std::string prefix, bool is_left);

	void require(Token, Types, char*);	//Type only
	template <typename T> void require(Token _t, T value, char* message)	//Value only
	{
		if (T(_t.value) != value)
			throw ParserError(message, _t.pos_line, _t.pos_col);
	}
	template <typename T> void require(Token _t, Types _type, T value, char* message)	//Both type and value
	{
		if (_t.type != _type || T(_t.value) != value)
			throw ParserError(message, _t.pos_line, _t.pos_col);
	}

	void print_def_decl_values(std::shared_ptr<_nodes::Node> node, std::string prefix);
	void print_vector(std::vector <std::shared_ptr<_nodes::Node>> vec, std::string prefix);
};