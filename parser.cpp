#include "parser.h"


void Parser::parse()
{
	try
	{
		root = parse_program();
	}
	catch (ParserError &e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (LexerError &e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (SemanticError &e)
	{
		std::cout << e.what() << std::endl;
	}
}

//CONSTRUCTOR / DESTRUCTOR
Parser::Parser(Lexer *l)
{
	lexer = l;
	symbol_table_stack.push_back(std::make_shared<Symbol_table>());

	//Pushing standart simple pascal types
	symbol_table_stack.back()->push(Symbol_table::Field::f_types, INTEGER, st_integer);
	symbol_table_stack.back()->push(Symbol_table::Field::f_types, FLOAT, st_float);
	symbol_table_stack.back()->push(Symbol_table::Field::f_types, CHAR, st_char);
	symbol_table_stack.back()->push(Symbol_table::Field::f_types, STRING, st_string);

	expressions_wo_check = false;
}
Parser::~Parser()
{

}


//		PRINT
void Parser::print()
{
	print_tree(root, "", false);
}

void Parser::print_tree(std::shared_ptr<_nodes::Node> node, std::string prefix, bool isleft)
{
	if (node == nullptr)
		return;

	std::cout << prefix << (isleft ? "|-- " : "|__ ");

	switch (node->type)
	{
	case Node_types::node_literal:
		switch (node->t.type)
		{
		case Types::int_type:
			std::cout << int(node->t.value);
			break;
		case Types::float_type:
			std::cout << double(node->t.value);
			break;
		case Types::char_type:
			std::cout << char(node->t.value);
			break;
		case Types::string_type:
			std::cout << std::string(node->t.value);
			break;
		}
		std::cout << std::endl;
		return;

	case Node_types::node_constant:
		std::cout << std::static_pointer_cast<_nodes::Constant>(node)->name << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::Constant>(node)->value, prefix, false);
		return;

	case Node_types::node_variable:
		std::cout << std::static_pointer_cast<_nodes::Variable>(node)->name << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::Variable>(node)->value, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::Variable>(node)->vtype, prefix, false);
		return;

	case Node_types::node_type:
		if (std::static_pointer_cast<_nodes::Type>(node)->name != NOTANAME)
			std::cout << std::static_pointer_cast<_nodes::Type>(node)->name << " : ";
		std::cout << std::static_pointer_cast<_nodes::Type>(node)->basic_type << std::endl;
		return;

	case Node_types::node_operator:	
		std::cout << operators_names.find(Operators(node->t.value))->second << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(node->child[0], prefix, true);
		print_tree(node->child[1], prefix, false);
		return;

	case Node_types::node_program:
		std::cout << "program" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::Program>(node)->program_heading, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::Program>(node)->block, prefix, false);
		return;

	case Node_types::node_program_heading:
		std::cout << std::static_pointer_cast<_nodes::Program_heading>(node)->heading << " //Program_heading" << std::endl;
		return;

	case Node_types::node_block:
		std::cout << "//Block" << std::endl;	//Do we need endl?
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::Block>(node)->declaration_part, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::Block>(node)->statement_part, prefix, false);
		return;

	case Node_types::node_declaration_part:
		std::cout << "//Declaration part" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_vector(std::static_pointer_cast<_nodes::Declaration_part>(node)->constants, prefix);
		print_vector(std::static_pointer_cast<_nodes::Declaration_part>(node)->variables, prefix);
		print_vector(std::static_pointer_cast<_nodes::Declaration_part>(node)->procedures, prefix);
		print_vector(std::static_pointer_cast<_nodes::Declaration_part>(node)->functions, prefix);
		print_vector(std::static_pointer_cast<_nodes::Declaration_part>(node)->types, prefix);
		return;

	case Node_types::node_constant_definition_part:
		std::cout << "const" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_def_decl_values(node, prefix);
		return;

	case Node_types::node_variable_declaration_part:
		std::cout << "var" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_def_decl_values(node, prefix);
		return;

	case Node_types::node_type_definition_part:
		std::cout << "type" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_def_decl_values(node, prefix);
		return;

	case Node_types::node_statement_part:
		std::cout << "//Statement part" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::Statement_part>(node)->sequence, prefix, false);
		return;

	case Node_types::node_statement_sequence:
		std::cout << "//Statement sequence" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_def_decl_values(node, prefix);
		return;

	case Node_types::node_assignment_statement:
		std::cout << "//Assignment_statement" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::Assignment_statement>(node)->variable, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::Assignment_statement>(node)->expression, prefix, false);
		return;

	case Node_types::node_typecast:
		std::cout << "//Typecast" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::Typecast>(node)->cast_type, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::Typecast>(node)->expression, prefix, false);
		return;

	case Node_types::node_procedure_declaration:
		std::cout << std::static_pointer_cast<_nodes::Procedure_declaration>(node)->name << "	//Procedure_declaration" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::Procedure_declaration>(node)->heading, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::Procedure_declaration>(node)->body, prefix, false);
		return;

	case Node_types::node_function_declaration:
		std::cout << std::static_pointer_cast<_nodes::Function_declaration>(node)->name << "	//Function_declaration" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::Function_declaration>(node)->ftype, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::Function_declaration>(node)->heading, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::Function_declaration>(node)->body, prefix, false);
		return;

	case Node_types::node_function_argument_list:
		std::cout << "//Arguments list" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_def_decl_values(node, prefix);
		return;

	case Node_types::node_function_call:
		std::cout << std::static_pointer_cast<_nodes::Function_declaration>(std::static_pointer_cast<_nodes::Function_call>(node)->function)->name << "//Function call" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::Function_declaration>(std::static_pointer_cast<_nodes::Function_call>(node)->function)->ftype, prefix, true);
		print_def_decl_values(node, prefix);
		return;

	case Node_types::node_procedure_statement:
		std::cout << std::static_pointer_cast<_nodes::Procedure_declaration>(std::static_pointer_cast<_nodes::Procedure_statement>(node)->procedure)->name << "//Procedure statement" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_def_decl_values(node, prefix);
		return;

	case Node_types::node_while_statement:
		std::cout << "while" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::While_statement>(node)->expression, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::While_statement>(node)->statement, prefix, false);
		return;
	case Node_types::node_repeat_statement:
		std::cout << "repeat" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::Repeat_statement>(node)->statements, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::Repeat_statement>(node)->expression, prefix, false);	
		return;
	case Node_types::node_for_statement:
		std::cout << "for";
		if (std::static_pointer_cast<_nodes::For_statement>(node)->downto)
			std::cout << " (downto)";
		std::cout << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::For_statement>(node)->variable, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::For_statement>(node)->start_expression, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::For_statement>(node)->finish_expression, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::For_statement>(node)->statement, prefix, false);
		return;
	case Node_types::node_write_statement:
		std::cout << "write" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::Write_statement>(node)->expression, prefix, false);
		return;
	case Node_types::node_if_statement:
		std::cout << "if" << std::endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		print_tree(std::static_pointer_cast<_nodes::If_statement>(node)->expression, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::If_statement>(node)->then_statement, prefix, true);
		print_tree(std::static_pointer_cast<_nodes::If_statement>(node)->else_statement, prefix, false);
		return;
	}
}

void Parser::print_def_decl_values(std::shared_ptr<_nodes::Node> node, std::string prefix)
{
	for (unsigned int i = 0; i < std::static_pointer_cast<_nodes::Def_decl_part>(node)->values.size(); i++)
		print_tree(std::static_pointer_cast<_nodes::Def_decl_part>(node)->values[i], prefix, i == 0 ? true : false);
}

void Parser::print_vector(std::vector <std::shared_ptr<_nodes::Node>> vec, std::string prefix)
{
	for (unsigned int i = 0; i < vec.size(); i++)
		print_tree(vec[i], prefix, i == 0 ? true : false);
}


//		UTILITY
void Parser::require(Token _t, Types _type, char* message)
{
	if (_t.type != _type)
		throw ParserError(message, _t.pos_line, _t.pos_col);
}

std::shared_ptr<_nodes::Node> Parser::stack_search(Symbol_table::Field field, std::string key)
{
	std::shared_ptr<_nodes::Node> sres;
	for (int i = symbol_table_stack.size() - 1; i >= 0; i--)
	{
		sres = symbol_table_stack[i]->get(field, key);
		if (sres != nullptr)
			return sres;
	}
	return nullptr;
}

bool Parser::id_is_duplicated(std::string id)
{
	std::shared_ptr<_nodes::Node> sres;

	Symbol_table::Field fields[5] = {
		Symbol_table::Field::f_constants, Symbol_table::Field::f_types, Symbol_table::Field::f_variables,
		Symbol_table::Field::f_functions, Symbol_table::Field::f_procedures
	};

	for (int i = 0; i < 5; i++)
		if (symbol_table_stack.back()->get(fields[i], id) != nullptr)
			return true;
	return false;
}

void Parser::expressions_test()
{
	expressions_wo_check = true;
	try
	{
		root = parse_expression();
	}
	catch (ParserError &e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (LexerError &e)
	{
		std::cout << e.what() << std::endl;
	}
}


//		PROGRAMS AND BLOCKS
std::shared_ptr<_nodes::Node> Parser::parse_program()
{
	std::shared_ptr<_nodes::Program> node = std::make_shared<_nodes::Program>();
	Token t;

	node->program_heading = parse_program_heading();
	node->block = parse_block();
	
	t = lexer->next();	//or current?
	require<Operators>(t, Types::operator_type, Operators::dot, "Parser error :: '.' expected");
	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_program_heading()
{
	std::shared_ptr<_nodes::Node> node;
	Token t;

	t = lexer->next();
	require<Keywords>(t, Types::keyword_type, Keywords::key_program, "Parser error :: 'program' expected");
	t = lexer->next();
	require(t, Types::id_type, "Parser error :: identifier expected");
	node = std::make_shared<_nodes::Program_heading>(t);
	t = lexer->next();
	require<Separators>(t, Types::separator_type, Separators::semicolon, "Parser error :: ';' expected");

	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_block()
{
	std::shared_ptr<_nodes::Block> node = std::make_shared<_nodes::Block>();
	Token t;

	symbol_table_stack.push_back(std::make_shared<Symbol_table>());

	node->declaration_part = parse_declaration_part();
	node->symbol_table = symbol_table_stack.back();
	t = lexer->current();
	require<Keywords>(t, Types::keyword_type, Keywords::key_begin, "Parser error :: 'begin' expected");
	node->statement_part = parse_statement_part();

	symbol_table_stack.pop_back();
	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_statement_part()
{
	std::shared_ptr<_nodes::Statement_part> node = std::make_shared<_nodes::Statement_part>();
	Token t;

	//We have already got 'begin' token
	node->sequence = parse_statement_sequence();
	t = lexer->current();	//Guess we'll need exactly the current token
	require<Keywords>(t, Types::keyword_type, Keywords::key_end, "Parser error :: 'end' expected");

	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_statement_sequence()
{
	std::shared_ptr<_nodes::Statement_sequence> node = std::make_shared<_nodes::Statement_sequence>();
	Token t;
	
	while (true)
	{
		node->values.push_back(parse_statement());

		if (node->values.back() == nullptr)
			t = lexer->current();
		else
			switch (node->values.back()->type)
			{
			case Node_types::node_assignment_statement:
			case Node_types::node_repeat_statement:
			case Node_types::node_for_statement:
				t = lexer->current();
				break;
			case Node_types::node_procedure_statement:
			case Node_types::node_statement_sequence:
			case Node_types::node_while_statement:
			case Node_types::node_write_statement:
			case Node_types::node_if_statement:
				t = lexer->next();
				break;
			}
		

		if (t.type == Types::separator_type)
			require<Separators>(t, Types::separator_type, Separators::semicolon, "Parser error :: ';' expected");
		else
			return node;	//In this case we meet an 'end' statement (require in the parse_statement_part function)
	}
	throw ParserError("Code error :: Unacceptable route in parse_statement_sequence", 0, 0);
}

std::shared_ptr<_nodes::Node> Parser::parse_statement()
{
	std::shared_ptr<_nodes::Node> node;
	Token t;

	t = lexer->next();

	switch (t.type)
	{
	case Types::id_type:	//it may be assignment or procedure
		if (stack_search(Symbol_table::Field::f_variables, std::string(t.value)) != nullptr)
			return parse_assignment_statement();
		else if (stack_search(Symbol_table::Field::f_procedures, std::string(t.value)) != nullptr)
			return parse_procedure_statement();
	case Types::keyword_type:
		switch (Keywords(t.value))
		{
		case Keywords::key_end:
		case Keywords::key_until:
			return nullptr;
		case Keywords::key_begin:
			return parse_compound_statement();
		case Keywords::key_while:
			return parse_while_statement();
		case Keywords::key_repeat:
			return parse_repeat_statement();
		case Keywords::key_for:
			return parse_for_statement();
		case Keywords::key_write:
			return parse_write_statement();
		case Keywords::key_if:
			return parse_if_statement();
		default:
			throw ParserError("Parser error :: unexpected keyword", t.pos_line, t.pos_col);
		
		}
	default:
		throw ParserError("Parser error :: Unacceptable token type", t.pos_line, t.pos_col);
	}
	
}

std::shared_ptr<_nodes::Node> Parser::parse_if_statement()
{
	std::shared_ptr<_nodes::If_statement> node = std::make_shared<_nodes::If_statement>();
	Token t;

	node->expression = parse_expression();

	if (get_type(node->expression) != st_integer)
		throw SemanticError("Semantic error :: Expression in 'if' statement must have 'integer' type", lexer->current().pos_line, lexer->current().pos_col);

	t = lexer->current();
	require<Keywords>(t, Types::keyword_type, Keywords::key_then, "Parser error :: 'then' expected");

	node->then_statement = parse_statement();

	switch (node->then_statement->type)
	{
	case Node_types::node_while_statement:
	case Node_types::node_assignment_statement:
	case Node_types::node_repeat_statement:
	case Node_types::node_if_statement:
		t = lexer->current();
		break;
	case Node_types::node_write_statement:
	case Node_types::node_compound_statement:
	case Node_types::node_for_statement:
	case Node_types::node_procedure_statement:
		t = lexer->next();
		break;
	}
	if (t.type == separator_type && Separators(t.value) == Separators::semicolon)
		return node;
	require<Keywords>(t, Types::keyword_type, Keywords::key_else, "Parser error :: 'else' expected");

	node->else_statement = parse_statement();
	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_write_statement()
{
	std::shared_ptr<_nodes::Write_statement> node = std::make_shared<_nodes::Write_statement>();
	Token t;

	t = lexer->next();
	require<Separators>(t, Types::separator_type, Separators::parenthesis_open, "Parser error :: '(' expected");

	node->expression = parse_expression();
	get_type(node->expression);	//Just to make sure types are correct
	t = lexer->current();
	require<Separators>(t, Types::separator_type, Separators::parenthesis_close, "Parser error :: ')' expected");
	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_for_statement()
{
	std::shared_ptr<_nodes::For_statement> node = std::make_shared<_nodes::For_statement>();
	Token t;

	t = lexer->next();
	require(t, Types::id_type, "Parser error :: Identificator expected");

	node->variable = stack_search(Symbol_table::Field::f_variables, std::string(t.value));
	if (get_type(node->variable) != st_integer)
		throw SemanticError("Semantic error :: Parameter of the 'for' statement should be integer variable", t.pos_line, t.pos_col);

	t = lexer->next();
	require<Operators>(t, Types::operator_type, Operators::assignment, "Parser error :: ':=' expected");

	node->start_expression = parse_expression();
	t = lexer->current();
	require(t, Types::keyword_type, "Parser error :: 'to' | 'downto' expected");

	if (Keywords(t.value) != Keywords::key_to && Keywords(t.value) != Keywords::key_downto)
		throw ParserError("Parser error :: 'to' | 'downto' expected", t.pos_line, t.pos_col);

	if (Keywords(t.value) == Keywords::key_downto)
		node->downto = true;

	node->finish_expression = parse_expression();

	t = lexer->current();
	require<Keywords>(t, Types::keyword_type, Keywords::key_do, "Parser error :: 'do' expected");

	node->statement = parse_statement();

	if (get_type(node->start_expression) != st_integer || get_type(node->finish_expression) != st_integer)
		throw SemanticError("Semantir error :: 'for' starting and finishing expressions should have 'integer' values", t.pos_line, t.pos_col);

	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_repeat_statement()
{
	std::shared_ptr<_nodes::Repeat_statement> node = std::make_shared<_nodes::Repeat_statement>();
	Token t;

	node->statements = parse_statement_sequence();

	t = lexer->current();
	require<Keywords>(t, Types::keyword_type, Keywords::key_until, "Parser error :: 'until' expected");

	node->expression = parse_expression();
	if (get_type(node->expression) != st_integer)
		throw SemanticError("Semantic error :: While condition should be 'integer' value", lexer->current().pos_line, lexer->current().pos_col);

	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_while_statement()
{
	std::shared_ptr<_nodes::While_statement> node = std::make_shared<_nodes::While_statement>();
	Token t;

	node->expression = parse_expression();

	if (get_type(node->expression) != st_integer)
		throw SemanticError("Semantic error :: While condition should be 'integer' value", lexer->current().pos_line, lexer->current().pos_col);

	t = lexer->current();

	require<Keywords>(t, Types::keyword_type, Keywords::key_do, "Parser error 'do' expected");

	node->statement = parse_statement();

	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_compound_statement()
{
	std::shared_ptr<_nodes::Node> node = parse_statement_sequence();
	Token t = lexer->current();
	require<Keywords>(t, Types::keyword_type, Keywords::key_end, "Parser error :: 'end' expected");
	return node;	
}

std::shared_ptr<_nodes::Node> Parser::parse_assignment_statement()
{
	std::shared_ptr<_nodes::Assignment_statement> node = std::make_shared<_nodes::Assignment_statement>();
	std::shared_ptr<_nodes::Node> var, exptype, vartype;
	std::shared_ptr<_nodes::Typecast> caster = std::make_shared<_nodes::Typecast>();
	Token t;

	t = lexer->current();
	
	var = stack_search(Symbol_table::Field::f_variables, std::string(t.value));
	if (var == nullptr)
		throw SemanticError("Semantic error :: Unknown identifier", t.pos_line, t.pos_col);
	//TODO We'll need to check if variable is an array or record
	//switch(var->vtype)

	node->variable = var;
	t = lexer->next();
	require<Operators>(t, Types::operator_type, Operators::assignment, "Parser error :: ':=' expected");
	node->expression = parse_expression();

	exptype = get_type(node->expression);
	vartype = std::static_pointer_cast<_nodes::Variable>(node->variable)->vtype;
	if (vartype != exptype)
		if (vartype == st_float && exptype == st_integer)
		{
			caster->cast_type = st_float;
			caster->expression = node->expression;
			node->expression = caster;
		}
		else
			throw SemanticError("Semantic error :: incompatible types in assignment statement", t.pos_line, t.pos_col);

		
	
	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_procedure_statement()
{
	std::shared_ptr<_nodes::Procedure_statement> node = std::make_shared<_nodes::Procedure_statement>();
	std::shared_ptr<_nodes::Procedure_declaration> procedure;
	std::shared_ptr<_nodes::Function_argument_list> heading;
	std::shared_ptr<_nodes::Node> type1, type2;
	std::shared_ptr<_nodes::Typecast> h;

	Token t = lexer->current();
	procedure = std::static_pointer_cast<_nodes::Procedure_declaration>(stack_search(Symbol_table::Field::f_procedures, std::string(t.value)));

	node->procedure = procedure;
	heading = std::static_pointer_cast<_nodes::Function_argument_list>(procedure->heading);

	t = lexer->next();
	require<Separators>(t, Types::separator_type, Separators::parenthesis_open, "Parser error :: '(' expected");

	for (unsigned int i = 0; i < heading->values.size(); i++)
	{
		node->values.push_back(parse_expression());
		type1 = get_type(node->values.back());
		type2 = get_type(heading->values[i]);
		if (type1 != type2)
		{
			if (type2 == st_float && type1 == st_integer)
			{
				h = std::make_shared<_nodes::Typecast>();
				h->cast_type = st_float;
				h->expression = node->values.back();
				node->values.back() = h;
			}
			else
				throw SemanticError("Semantic error :: Types doesn't match in procedure call", lexer->current().pos_line, lexer->current().pos_col);
		}

		t = lexer->current();
		if (i < heading->values.size() - 1)
			require<Separators>(t, Types::separator_type, Separators::comma, "Parser error :: ',' expected");
		else
			require<Separators>(t, Types::separator_type, Separators::parenthesis_close, "Parser error :: ')' expected");
	}

	if (heading->values.size() == 0)
	{
		t = lexer->next();
		require<Separators>(t, Types::separator_type, Separators::parenthesis_close, "Parser error :: ')' expected");
	}

	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_declaration_part()
{
	std::shared_ptr<_nodes::Declaration_part> node = std::make_shared<_nodes::Declaration_part>();
	Token t;

	bool need_to_read = true;

	while (true)
	{
		if (need_to_read)
			t = lexer->next();
		else
		{
			need_to_read = true;
			t = lexer->current();
		}

		require(t, Types::keyword_type, "Parser error :: Keyword expected");
		switch (Keywords(t.value))
		{
		case Keywords::key_const:
			node->constants.push_back(parse_constant_definition_part());
			need_to_read = false;
			break;
		case Keywords::key_type:
			node->types.push_back(parse_type_definition_part());
			need_to_read = false;
			break;
		case Keywords::key_var:
			node->variables.push_back(parse_variable_declaration_part());
			need_to_read = false;
			break;
		case Keywords::key_procedure:
			node->procedures.push_back(parse_procedure_declaration());
			need_to_read = true;
			break;
		case Keywords::key_function:
			node->functions.push_back(parse_function_declaration());
			need_to_read = true;
			break;
		case Keywords::key_begin:
			return node;
		default:
			throw ("Parser error :: unacceptable keyword", t.pos_line, t.pos_col);
		}
	}
	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_function_declaration()
{
	std::shared_ptr<_nodes::Function_declaration> node = std::make_shared<_nodes::Function_declaration>();
	Token t;

	//We have already got 'function' token
	t = lexer->next();
	require(t, Types::id_type, "Parser error :: Identificator expected");

	if (id_is_duplicated(std::string(t.value)))
		throw SemanticError("Semantic error :: Identificator duplication in function declaration", t.pos_line, t.pos_col);

	node->name = std::string(t.value);

	t = lexer->next();
	require<Separators>(t, Types::separator_type, Separators::parenthesis_open, "Parser error :: '(' expected");

	symbol_table_stack.push_back(std::make_shared<Symbol_table>());
	node->symbol_table = symbol_table_stack.back();

	node->heading = parse_function_arguments_list();

	t = lexer->current();
	require<Separators>(t, Types::separator_type, Separators::parenthesis_close, "Parser error :: ')' expected");
	t = lexer->next();
	require<Separators>(t, Types::separator_type, Separators::colon, "Parser error :: ':' expected");

	node->ftype = parse_type();
	t = lexer->next();
	require<Separators>(t, Types::separator_type, Separators::semicolon, "Parser error :: ';' expected");

	node->body = parse_block();	//Block creates new symbol table for himself and pops it

	symbol_table_stack.pop_back();

	symbol_table_stack.back()->push(Symbol_table::Field::f_functions, node->name, node);

	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_procedure_declaration()
{
	std::shared_ptr<_nodes::Procedure_declaration> node = std::make_shared<_nodes::Procedure_declaration>();
	Token t;

	//We have already got 'procedure' token
	t = lexer->next();
	require(t, Types::id_type, "Parser error :: Identificator expected");

	if (id_is_duplicated(std::string(t.value)))
		throw SemanticError("Semantic error :: Identificator duplication in procedure declaration", t.pos_line, t.pos_col);

	node->name = std::string(t.value);

	t = lexer->next();
	require<Separators>(t, Types::separator_type, Separators::parenthesis_open, "Parser error :: '(' expected");

	symbol_table_stack.push_back(std::make_shared<Symbol_table>());
	node->symbol_table = symbol_table_stack.back();

	node->heading = parse_function_arguments_list();

	t = lexer->current();
	require<Separators>(t, Types::separator_type, Separators::parenthesis_close, "Parser error :: ')' expected");
	t = lexer->next();
	require<Separators>(t, Types::separator_type, Separators::semicolon, "Parser error :: ';' expected");

	node->body = parse_block();	//Block creates new symbol table for himself and pops it

	symbol_table_stack.pop_back();

	symbol_table_stack.back()->push(Symbol_table::Field::f_procedures, node->name, node);

	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_function_arguments_list()
{
	std::shared_ptr<_nodes::Function_argument_list> node = std::make_shared<_nodes::Function_argument_list>();
	Token t;
	std::shared_ptr<_nodes::Node> typenode;
	std::shared_ptr<_nodes::Node> h;

	std::vector<std::string> identifier_list;

	while (true)
	{
		identifier_list = parse_identifier_list();

		if (identifier_list.size() == 0)
			return node;

		//We've got ':' already
		typenode = parse_type();

		for (unsigned int i = 0; i < identifier_list.size(); i++)
		{
			h = std::make_shared<_nodes::Variable>(identifier_list[i], typenode);
			symbol_table_stack.back()->push(Symbol_table::Field::f_variables, identifier_list[i], h);
			node->values.push_back(h);
		}

		t = lexer->next();
		if (t.type != Types::separator_type || (t.type == Types::separator_type && Separators(t.value) != Separators::semicolon))
			return node;

		identifier_list.clear();

	}

	throw ParserError("Code error :: unacceptable route in parse_function_arguments_list", 0, 0);
}

std::shared_ptr<_nodes::Node> Parser::parse_constant_definition_part()
{
	std::shared_ptr<_nodes::Constant_definition_part> node = std::make_shared<_nodes::Constant_definition_part>();
	std::shared_ptr<_nodes::Node> h, expr;
	Token t;
	std::string id;

	//we have already got 'const' token

	t = lexer->next();
	require(t, Types::id_type, "Parser error :: identifier expected");	//First one MUST be identifier

	while (t.type == Types::id_type)
	{
		id = std::string(t.value);

		if (id_is_duplicated(id))
			throw SemanticError("Semantic error :: Id duplication in constant definition", t.pos_line, t.pos_col);

		t = lexer->next();
		require<Operators>(t, Types::operator_type, Operators::equal, "Parser error :: '=' expected");
		expr = parse_expression();

		h = std::make_shared<_nodes::Constant>(id, expr, get_type(expr));
		symbol_table_stack.back()->push(Symbol_table::Field::f_constants, id, h);
		node->values.push_back(h);
		t = lexer->current();
		require<Separators>(t, Types::separator_type, Separators::semicolon, "Parser error :: ';' expected");

		t = lexer->next();
	}

	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_type_definition_part()
{
	std::shared_ptr<_nodes::Type_definition_part> node = std::make_shared<_nodes::Type_definition_part>();
	std::shared_ptr<_nodes::Node> type;
	Token t;
	std::string id;

	//we have already got 'type' token

	t = lexer->next();
	require(t, Types::id_type, "Parser error :: identifier expected");	//First one MUST be identifier

	while (t.type == Types::id_type)
	{
		id = std::string(t.value);

		if (id_is_duplicated(id))
			throw SemanticError("Semantic error :: Id duplication in type definition", t.pos_line, t.pos_col);

		t = lexer->next();
		require<Operators>(t, Types::operator_type, Operators::equal, "Parser error :: '=' expected");
		type = parse_type();
		
		std::static_pointer_cast<_nodes::Type>(type)->name = id;

		symbol_table_stack.back()->push(Symbol_table::Field::f_types, id, type);

		node->values.push_back(type);
		t = lexer->next();	//In const_def there are a 'current'
		require<Separators>(t, Types::separator_type, Separators::semicolon, "Parser error :: ';' expected");

		t = lexer->next();
	}

	return node;
}

std::vector<std::string> Parser::parse_identifier_list()
{
	std::vector<std::string> res;
	Token t;

	while (true)
	{
		t = lexer->next();
		if (t.type != Types::id_type)	//TOTHINK still not confident about that variant
			return res;
		//require(t, Types::id_type, "Parser error :: identifier expected");

		if (id_is_duplicated(std::string(t.value)))
			throw SemanticError("Semantic error :: Id duplication", t.pos_line, t.pos_col);

		res.push_back(std::string(t.value));

		t = lexer->next();
		require(t, Types::separator_type, "Parser error :: separator expected");
		switch (Separators(t.value))
		{
		case Separators::comma:
			break;
		case Separators::colon:
			return res;
		default:
			throw ParserError("Parser error :: ':' expected", t.pos_line, t.pos_col);
		}
	}
	throw ParserError("Code error :: Unacceptable route in parse_identifier_list", 0, 0);
}

std::shared_ptr<_nodes::Node> Parser::parse_variable_declaration_part()
{
	std::shared_ptr<_nodes::Variable_declaration_part> node = std::make_shared<_nodes::Variable_declaration_part>();
	Token t;
	std::shared_ptr<_nodes::Node> typenode;
	std::shared_ptr<_nodes::Node> h;

	bool not_a_first_time = false;

	//we've got 'var' already

	std::vector<std::string> identifier_list;
	
	while (true)
	{
		identifier_list = parse_identifier_list();

		if (not_a_first_time && identifier_list.size() == 0)
			return node;
		
		//We've got ':' already
		typenode = parse_type();

		//TODO standart value will be here, don't forget about it
		t = lexer->next();
		require<Separators>(t, Types::separator_type, Separators::semicolon, "Parser error :: ';' expected");
		//^^^^^^^^^^^^^^^^^^^^

		for (unsigned int i = 0; i < identifier_list.size(); i++)
		{
			h = std::make_shared<_nodes::Variable>(identifier_list[i], typenode);
			symbol_table_stack.back()->push(Symbol_table::Field::f_variables, identifier_list[i], h);
			node->values.push_back(h);
		}

		identifier_list.clear();

		not_a_first_time = true;
	}
	
	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_type()
{
	std::shared_ptr<_nodes::Node> node;
	Token t;
	
	t = lexer->next();
	switch (t.type)
	{
	case Types::id_type:
		node = stack_search(Symbol_table::Field::f_types, std::string(t.value));
		if (node == nullptr)
			throw SemanticError("Semantic error :: Unknown type identifier", t.pos_line, t.pos_col);
		return node;
	//TODO parse arrays, records etc
	case Types::keyword_type:
		//placeholder
		
	default:
		throw ParserError("Parser error :: Unacceptable token type", t.pos_line, t.pos_col);
	}

	return nullptr;
}


//		EXPRESSIONS
std::shared_ptr<_nodes::Node> Parser::parse_expression(int mode)	
{
	std::shared_ptr<_nodes::Node> node, up;
	Token t;

	if (mode == 2)
		if (expressions_wo_check)
			node = parse_factor_WO_check();
		else
			node = parse_factor();
	else
		node = parse_expression(mode + 1);

	if (mode == 2)
		t = lexer->next();
	else
		t = lexer->current();
	while (true)
	{
		switch (mode)
		{
		case 0:
			if (t.type == Types::operator_type && relational_operators.count(Operators(t.value)))
				break;
			else
				return node;
		case 1:
			if (t.type == Types::operator_type && addition_operators.count(Operators(t.value)))
				break;
			else
				return node;
		case 2:
			if (t.type == Types::operator_type && multiplication_operators.count(Operators(t.value)))
				break;
			else
				return node;
		}

		up = std::make_shared<_nodes::Operator>(t);
		up->child[0] = node;
		if (mode == 2)
			if (expressions_wo_check)
				up->child[1] = parse_factor_WO_check();
			else
				up->child[1] = parse_factor();
		else
			up->child[1] = parse_expression(mode + 1);
		node = up;

		if (!expressions_wo_check)
			if (node->child[0]->type == Node_types::node_literal && node->child[1]->type == Node_types::node_literal)
			{
				switch (Operators(up->t.value))
				{
				case Operators::addition:
					node = up->child[0] + up->child[1];
					break;
				case Operators::substraction:
					node = up->child[0] - up->child[1];
					break;
				case Operators::division:
					node = up->child[0] / up->child[1];
					break;
				case Operators::multiplication:
					node = up->child[0] * up->child[1];
					break;
				case Operators::integer_division:
					node = operator_div(up->child[0], up->child[1]);
					break;
				case Operators::remainder_operator:
					node = operator_mod(up->child[0], up->child[1]);
					break;
				case Operators::unary_minus:
					node = operator_unary_minus(up->child[0]);
					break;
				}
			}

		if (mode == 2)
			t = lexer->next();
		else
			t = lexer->current();
	}

	return node;
}

std::shared_ptr<_nodes::Node> Parser::parse_factor()
{
	std::shared_ptr<_nodes::Node> node, sres;
	Token t, t_sec;

	t = lexer->next();
	switch (t.type)
	{
	case int_type:
		return std::make_shared<_nodes::Literal>(t, st_integer);
	case float_type:
		return std::make_shared<_nodes::Literal>(t, st_float);
	case char_type:
		return std::make_shared<_nodes::Literal>(t, st_char);
	case string_type:
		return std::make_shared<_nodes::Literal>(t, st_string);
	case id_type:
		sres = stack_search(Symbol_table::Field::f_constants, std::string(t.value));
		if (sres != nullptr)
			return std::static_pointer_cast<_nodes::Constant>(sres)->value;	//Value should be literal?

		sres = stack_search(Symbol_table::Field::f_variables, std::string(t.value));
		if (sres != nullptr)
			return sres;

		sres = stack_search(Symbol_table::Field::f_procedures, std::string(t.value));
		if (sres != nullptr)
			throw ParserError("Parser error :: Procedure using in expressions is restricted", t.pos_line, t.pos_col);

		sres = stack_search(Symbol_table::Field::f_functions, std::string(t.value));
		if (sres != nullptr)
			return parse_function_call();
		
		throw ParserError("Parser error :: Unknown identifier", t.pos_line, t.pos_col);

	case Types::end_of_file:
		throw ParserError("Parser error :: Unexpected end of file", t.pos_line, t.pos_col);

	case Types::separator_type:
		require<Separators>(t, Separators::parenthesis_open, "Parser error :: Unexpected separator while reading expression");
		node = parse_expression();
		t_sec = lexer->current();
		require<Separators>(t_sec, Separators::parenthesis_close, "Parser error :: ')' expected");
		return node;

	case Types::operator_type:
		if (Operators(t.value) == Operators::bitwise_negation)
		{
			node = std::make_shared<_nodes::Operator>(t);
			node->child[0] = parse_factor();
			return node;
		}
		else if (Operators(t.value) == Operators::substraction)
		{
			t.value = Operators::unary_minus;
			node = std::make_shared<_nodes::Operator>(t);
			node->child[0] = parse_factor();
			if (node->child[0]->type == Node_types::node_literal)
				node = operator_unary_minus(node->child[0]);
			return node;
		}
	
		else
			throw ParserError("Parser error :: Unexpected operator while reading expression", t.pos_line, t.pos_col);

	default:
		throw ParserError("Parser error :: Unacceptable token type", t.pos_line, t.pos_col);
	}

	return nullptr;
}

std::shared_ptr<_nodes::Node> Parser::parse_factor_WO_check()
{
	std::shared_ptr<_nodes::Node> node;
	Token t, t_sec;

	t = lexer->next();

	switch (t.type)
	{
	case int_type:
		return std::make_shared<_nodes::Literal>(t, st_integer);
	case float_type:
		return std::make_shared<_nodes::Literal>(t, st_float);
	case char_type:
		return std::make_shared<_nodes::Literal>(t, st_char);
	case string_type:
		return std::make_shared<_nodes::Literal>(t, st_string);

	case Types::end_of_file:
		throw ParserError("Parser error :: Unexpected end of file", t.pos_line, t.pos_col);

	case Types::separator_type:
		require<Separators>(t, Separators::parenthesis_open, "Parser error :: Unexpected separator while reading expression");
		node = parse_expression(true);
		t_sec = lexer->current();
		require<Separators>(t_sec, Separators::parenthesis_close, "Parser error :: ')' expected");
		return node;

	case Types::operator_type:
		if (Operators(t.value) == Operators::bitwise_negation)
		{
			node = std::make_shared<_nodes::Operator>(t);
			node->child[0] = parse_factor_WO_check();
			return node;
		}
		else if (Operators(t.value) == Operators::substraction)
		{
			t.value = Operators::unary_minus;
			node = std::make_shared<_nodes::Operator>(t);
			node->child[0] = parse_factor_WO_check();
			return node;
		}
		else
			throw ParserError("Parser error :: Unexpected operator while reading expression", t.pos_line, t.pos_col);

	default:
		throw ParserError("Parser error :: Unacceptable token type", t.pos_line, t.pos_col);
	}
}

std::shared_ptr<_nodes::Node> Parser::parse_function_call()
{
	std::shared_ptr<_nodes::Function_call> node = std::make_shared<_nodes::Function_call>();
	std::shared_ptr<_nodes::Function_declaration> function;
	std::shared_ptr<_nodes::Function_argument_list> heading;
	std::shared_ptr<_nodes::Node> type1, type2;
	std::shared_ptr<_nodes::Typecast> h;

	Token t = lexer->current();
	function = std::static_pointer_cast<_nodes::Function_declaration>(stack_search(Symbol_table::Field::f_functions, std::string(t.value)));

	node->function = function;
	heading = std::static_pointer_cast<_nodes::Function_argument_list>(function->heading);

	t = lexer->next();
	require<Separators>(t, Types::separator_type, Separators::parenthesis_open, "Parser error :: '(' expected");

	for (unsigned int i = 0; i < heading->values.size(); i++)
	{
		node->values.push_back(parse_expression());
		type1 = get_type(node->values.back());
		type2 = get_type(heading->values[i]);
		if (type1 != type2)
		{
			if (type2 == st_float && type1 == st_integer)
			{
				h = std::make_shared<_nodes::Typecast>();
				h->cast_type = st_float;
				h->expression = node->values.back();
				node->values.back() = h;
			}
			else
				throw SemanticError("Semantic error :: Types doesn't match in function call", lexer->current().pos_line, lexer->current().pos_col);
		}

		t = lexer->current();
		if (i < heading->values.size() - 1)
			require<Separators>(t, Types::separator_type, Separators::comma, "Parser error :: ',' expected");
		else
			require<Separators>(t, Types::separator_type, Separators::parenthesis_close, "Parser error :: ')' expected");
	}

	if (heading->values.size() == 0)
	{
		t = lexer->next();
		require<Separators>(t, Types::separator_type, Separators::parenthesis_close, "Parser error :: ')' expected");
	}

	return node;
}