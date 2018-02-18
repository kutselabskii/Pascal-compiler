#pragma once

#include <map>


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
	read_operator_end
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
};

enum Operators
{
	//Arithmetic
	addition,
	substraction,
	multiplication,
	division,
	integer_division,
	remainder,
	//Logical
	bitwise_negation,
	bitwise_and,
	bitwise_or,
	bitwise_xor,
	bitwise_shift_to_the_left,
	bitwise_shift_to_the_right,
	//Relational
	equal,
	not_equal,
	strictly_less_than,
	strictly_greater_than,
	less_than_or_equal,
	greater_than_or_equal
};

const std::map <std::string, Operators> operators_list = {
	{"+", Operators::addition}, {"-", Operators::substraction}, {"*", Operators::multiplication}, {"/", Operators::division}, {"div", Operators::integer_division},
	{"mod", Operators::remainder},  {"not", Operators::bitwise_negation}, {"and", Operators::bitwise_and}, {"or", Operators::bitwise_or},
	{"xor", Operators::bitwise_xor}, {"shl", Operators::bitwise_shift_to_the_left}, {"shr", Operators::bitwise_shift_to_the_right},
	{"<<", Operators::bitwise_shift_to_the_left}, {">>", Operators::bitwise_shift_to_the_right}, {"=", Operators::equal}, {"<>", Operators::not_equal},
	{"<", Operators::strictly_less_than}, {">", Operators::strictly_greater_than}, {"<=", Operators::less_than_or_equal}, {">=", Operators::greater_than_or_equal}
};

const std::map <Operators, std::string> operators_names = {
	{Operators::addition, "addition"}, {Operators::substraction, "substraction"}, {Operators::multiplication, "multiplication"}, {Operators::division, "division"}, 
	{Operators::integer_division, "integer_division"}, {Operators::remainder, "remainder"},  {Operators::bitwise_negation, "bitwise_negation"}, 
	{Operators::bitwise_and, "bitwise_and"}, {Operators::bitwise_or, "bitwise_or"}, {Operators::bitwise_xor, "bitwise_xor"}, 
	{Operators::bitwise_shift_to_the_left, "bitwise_shift_to_the_left"}, {Operators::bitwise_shift_to_the_right, "bitwise_shift_to_the_right"},
	{Operators::equal, "equal"}, {Operators::not_equal, "not_equal"}, {Operators::strictly_less_than, "strictly_less_than"}, 
	{Operators::strictly_greater_than, "strictly_greater_than"}, {Operators::less_than_or_equal, "less_than_or_equal"}, 
	{Operators::greater_than_or_equal, "greater_than_or_equal"}
};

enum Separators
{
	parenthesis_open,
	parenthesis_close,
	square_bracket_open,
	square_bracket_close,
	semicolon
};

const std::map <std::string, Separators> separators_list = {
	{"(", Separators::parenthesis_open}, {")", Separators::parenthesis_close}, {"[", Separators::square_bracket_open}, {"]", Separators::square_bracket_close},
	{";", Separators::semicolon}
};

const std::map <Separators, std::string> separators_names = {
	{Separators::parenthesis_open, "parenthesis_open" }, {Separators::parenthesis_close, "parenthesis_close" },
	{Separators::square_bracket_open, "square_bracket_open" }, {Separators::square_bracket_close, "square_bracket_close" }, {Separators::semicolon, "semicolon" }
};
