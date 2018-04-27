#pragma once

#include <map>
#include <set>


enum States
{
	unknown_symbol,
	whitespace,
	comment_multiline_bracket_start,
	sharp_char_start,
	line_end,
	dollar_hex_start,
	string_start,
	semicolon_open,
	comment_multiline_legacy_line_end,
	asterisk_end_legacy_comment,
	read_operator,
	read_int_num,
	read_separator,
	read_id,
	comment_bracket_line_end,
	unexpected_line_end,
	sharp_char_read,
	dollar_hex_read,
	comment_multiline_bracket_read,
	comment_multiline_bracket_end,
	string_read,
	string_end,
	comment_multiline_legacy_start,
	comment_multiline_legacy_read,
	comment_multiline_legacy_end,
	int_num_end,
	read_id_end,
	read_separator_end,
	sharp_char_end,
	dollar_hex_end,
	maybe_quote_symbol_1,
	string_with_char_start_char,
	string_with_char_start_string,
	read_real_num_start,
	maybe_quote_symbol_2,
	read_scale_factor_start,
	read_real_num,
	read_scale_factor_read,
	read_scale_factor_continue,
	real_num_end,
	read_operator_end,
	maybe_quote_symbol_3,
};

enum Types
{
	unknown_character,
	end_of_file,
	int_type,
	float_type,
	string_type,
	id_type,
	operator_type,
	separator_type,
	char_type,
	keyword_type,
};

enum Operators
{
	//Arithmetic
	addition, substraction, multiplication, division, integer_division, remainder_operator, unary_minus,
	//Logical
	bitwise_negation, bitwise_and, bitwise_or, bitwise_xor, bitwise_shift_to_the_left, bitwise_shift_to_the_right,
	//Relational
	equal, not_equal, strictly_less_than, strictly_greater_than, less_than_or_equal, greater_than_or_equal,
	//???
	dot, assignment
};

const std::set<Operators> relational_operators = {
	Operators::equal, Operators::not_equal, Operators::strictly_less_than, Operators::strictly_greater_than,
	Operators::less_than_or_equal, Operators::greater_than_or_equal
};

const std::set<Operators> addition_operators = {
	Operators::addition, Operators::substraction, Operators::bitwise_or, Operators::bitwise_xor
};

const std::set<Operators> multiplication_operators = {
	Operators::multiplication, Operators::division, Operators::integer_division, Operators::remainder_operator, Operators::bitwise_and
};

const std::map <std::string, Operators> operators_list = {
	{"+", Operators::addition}, {"-", Operators::substraction}, {"*", Operators::multiplication}, {"/", Operators::division}, {"div", Operators::integer_division},
	{"mod", Operators::remainder_operator},  {"not", Operators::bitwise_negation}, {"and", Operators::bitwise_and}, {"or", Operators::bitwise_or},
	{"xor", Operators::bitwise_xor}, {"shl", Operators::bitwise_shift_to_the_left}, {"shr", Operators::bitwise_shift_to_the_right},
	{"<<", Operators::bitwise_shift_to_the_left}, {">>", Operators::bitwise_shift_to_the_right}, {"=", Operators::equal}, {"<>", Operators::not_equal},
	{"<", Operators::strictly_less_than}, {">", Operators::strictly_greater_than}, {"<=", Operators::less_than_or_equal}, {">=", Operators::greater_than_or_equal},
	{":=", Operators::assignment}, { ".", Operators::dot }
};

const std::map <Operators, std::string> operators_names = {
	{Operators::addition, "addition"}, {Operators::substraction, "substraction"}, {Operators::multiplication, "multiplication"}, {Operators::division, "division"}, 
	{Operators::integer_division, "integer_division"}, {Operators::remainder_operator, "remainder"},  {Operators::bitwise_negation, "bitwise_negation"}, 
	{Operators::bitwise_and, "bitwise_and"}, {Operators::bitwise_or, "bitwise_or"}, {Operators::bitwise_xor, "bitwise_xor"}, 
	{Operators::bitwise_shift_to_the_left, "bitwise_shift_to_the_left"}, {Operators::bitwise_shift_to_the_right, "bitwise_shift_to_the_right"},
	{Operators::equal, "equal"}, {Operators::not_equal, "not_equal"}, {Operators::strictly_less_than, "strictly_less_than"}, 
	{Operators::strictly_greater_than, "strictly_greater_than"}, {Operators::less_than_or_equal, "less_than_or_equal"}, 
	{Operators::greater_than_or_equal, "greater_than_or_equal"}, {Operators::assignment, "assignment"}, {Operators::dot, "dot"}, {Operators::unary_minus, "unary_minus"}
};

enum Separators
{
	parenthesis_open, parenthesis_close, square_bracket_open, square_bracket_close, semicolon, colon, comma
};

const std::map <std::string, Separators> separators_list = {
	{"(", Separators::parenthesis_open}, {")", Separators::parenthesis_close}, {"[", Separators::square_bracket_open}, {"]", Separators::square_bracket_close},
	{";", Separators::semicolon}, {":", Separators::colon}, {",", Separators::comma}
};

const std::map <Separators, std::string> separators_names = {
	{Separators::parenthesis_open, "parenthesis_open" }, {Separators::parenthesis_close, "parenthesis_close" },
	{Separators::square_bracket_open, "square_bracket_open" }, {Separators::square_bracket_close, "square_bracket_close" }, 
	{Separators::semicolon, "semicolon" }, {Separators::colon, "colon"}, {Separators::comma, "comma"}
};

enum Keywords
{
	key_and, key_array, key_begin, key_break, key_const, key_continue, key_div, key_do, key_downto, key_else, key_end,
	key_false, key_for, key_function, key_if, key_mod, key_nil, key_not, key_of, key_or, key_procedure, key_program,
	key_record, key_repeat, key_shl, key_shr, key_then, key_to, key_true, key_type, key_var, key_while, key_xor, key_until, key_write
};

const std::map <std::string, Keywords> keywords_list = {
	{"and", Keywords::key_and}, {"array", Keywords::key_array}, {"begin", Keywords::key_begin}, {"break", Keywords::key_break},
	{"const", Keywords::key_const}, {"continue", Keywords::key_continue}, {"div", Keywords::key_div}, {"do", Keywords::key_do},
	{"downto", Keywords::key_downto}, {"else", Keywords::key_else}, {"end", Keywords::key_end}, {"false", Keywords::key_false},
	{"for", Keywords::key_for}, {"function", Keywords::key_function}, {"if", Keywords::key_if}, {"mod", Keywords::key_mod},
	{"nil", Keywords::key_nil}, {"not", Keywords::key_not}, {"of", Keywords::key_of}, {"or", Keywords::key_or},
	{"procedure", Keywords::key_procedure}, {"program", Keywords::key_program}, {"record", Keywords::key_record}, 
	{"repeat", Keywords::key_repeat}, {"shl", Keywords::key_shl}, {"shr", Keywords::key_shr}, {"then", Keywords::key_then},
	{"to", Keywords::key_to}, {"true", Keywords::key_true}, {"type", Keywords::key_type}, {"var", Keywords::key_var},
	{"while", Keywords::key_while}, {"xor", Keywords::key_xor}, {"until", Keywords::key_until}, {"write", Keywords::key_write}
};

const std::map <Keywords, std::string> keywords_names = {
	{ Keywords::key_and, "and" },{ Keywords::key_array, "array" },{ Keywords::key_begin, "begin" },{ Keywords::key_break, "break" },
	{ Keywords::key_const, "const" },{ Keywords::key_continue, "continue" },{ Keywords::key_div, "div" },{ Keywords::key_do, "do" },
	{ Keywords::key_downto, "downto" },{ Keywords::key_else, "else" },{ Keywords::key_end, "end" },{ Keywords::key_false, "false" },
	{ Keywords::key_for, "for" },{ Keywords::key_function, "function" },{ Keywords::key_if, "if" },{ Keywords::key_mod, "mod" },
	{ Keywords::key_nil, "nil" },{ Keywords::key_not, "not" },{ Keywords::key_of, "of" },{ Keywords::key_or, "or" },
	{ Keywords::key_procedure, "procedure" },{ Keywords::key_program, "program" },{ Keywords::key_record, "record" },
	{ Keywords::key_repeat, "repeat" },{ Keywords::key_shl, "shl" },{ Keywords::key_shr, "shr" },{ Keywords::key_then, "then" },
	{ Keywords::key_to, "to" },{ Keywords::key_true, "true" },{ Keywords::key_type, "type" },{ Keywords::key_var, "var" },
	{ Keywords::key_while, "while" },{ Keywords::key_xor, "xor" },{Keywords::key_until, "until"},{Keywords::key_write, "write"}
};

enum Node_types
{
	node_node,
	node_program,
	node_program_heading,
	node_block,
	node_declaration_part,
	node_constant_definition_part,
	node_variable_declaration_part,
	node_type_definition_part,
	node_procedure_declaration,
	node_function_declaration,
	node_function_argument_list,
	node_function_call,
	node_procedure_statement,
	node_compound_statement,
	node_while_statement,
	node_repeat_statement,
	node_for_statement,
	node_write_statement,
	node_if_statement,
	node_statement_part,
	node_constant,
	node_variable,
	node_literal,
	node_type,
	node_operator,
	node_typecast,

	node_statement_sequence,
	node_assignment_statement,
};

enum Asm_commands
{
	pop,
	push,
	enter,
	mov,
	lea,
	add,
	sub,
	imul,
	idiv,
	cdq,
	xor,
	cmp,
	and,
	or,
	c_xor,
	test
};

const std::map<Asm_commands, std::string> commands_to_string = {
	{Asm_commands::pop, "pop"}, {Asm_commands::push, "push"}, {Asm_commands::enter, "enter"}, {Asm_commands::mov, "mov"}, {Asm_commands::lea, "lea"},
	{Asm_commands::add, "add"}, {Asm_commands::sub, "sub"}, {Asm_commands::imul, "imul"}, {Asm_commands::idiv, "idiv"}, {Asm_commands::cdq, "cdq"},
	{Asm_commands::xor, "xor"}, {Asm_commands::cmp, "cmp"}, {Asm_commands::and, "and"}, {Asm_commands::or, "or"}, {Asm_commands::c_xor, "xor"},
	{Asm_commands::test, "test"}
};

enum Registers
{
	EAX,
	EBX,
	ECX,
	EDX,
	EBP,
	ESP
};

const std::map<Registers, std::string> register_to_string = {
	{EAX, "eax"}, {EBX, "ebx"}, {ECX, "ecx"}, {EDX, "edx"}, {EBP, "ebp"}, {ESP, "esp"}
};
