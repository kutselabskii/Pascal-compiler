#pragma once

#include "parser.h"

std::shared_ptr<_nodes::Node> operator+(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right);
std::shared_ptr<_nodes::Node> operator-(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right);
std::shared_ptr<_nodes::Node> operator/(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right);
std::shared_ptr<_nodes::Node> operator*(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right);
std::shared_ptr<_nodes::Node> operator_div(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right);
std::shared_ptr<_nodes::Node> operator_mod(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right);
std::shared_ptr<_nodes::Node> operator_unary_minus(std::shared_ptr<_nodes::Node> _left);

std::shared_ptr<_nodes::Node> get_type(std::shared_ptr<_nodes::Node> node);

const std::map<std::shared_ptr<_nodes::Node>, std::map<std::shared_ptr<_nodes::Node>, std::shared_ptr<_nodes::Node>>> type_table_addition = 
{
	{ st_integer,  { { st_integer, st_integer }, {st_float, st_float}, {st_char, nullptr}, {st_string, nullptr} } },
	{ st_float, { { st_integer, st_float}, {st_float, st_float}, {st_char, nullptr}, {st_string, nullptr} } },
	{ st_char, { { st_integer, nullptr}, {st_float, nullptr}, {st_char, st_string}, {st_string, st_string} } },
	{ st_string, { { st_integer, nullptr}, {st_float, nullptr}, {st_char, st_string}, {st_string, st_string} } }
};

const std::map<std::shared_ptr<_nodes::Node>, std::map<std::shared_ptr<_nodes::Node>, std::shared_ptr<_nodes::Node>>> type_table_division =
{
	{ st_integer,{ { st_integer, st_float },{ st_float, st_float },{ st_char, nullptr },{ st_string, nullptr } } },
	{ st_float,{ { st_integer, st_float },{ st_float, st_float },{ st_char, nullptr },{ st_string, nullptr } } },
	{ st_char,{ { st_integer, nullptr },{ st_float, nullptr },{ st_char, nullptr },{ st_string, nullptr } } },
	{ st_string,{ { st_integer, nullptr },{ st_float, nullptr },{ st_char, nullptr },{ st_string, nullptr } } }
};

const std::map<std::shared_ptr<_nodes::Node>, std::map<std::shared_ptr<_nodes::Node>, std::shared_ptr<_nodes::Node>>> type_table_remainder =
{
	{ st_integer,{ { st_integer, st_integer },{ st_float, nullptr },{ st_char, nullptr },{ st_string, nullptr } } },
	{ st_float,{ { st_integer, nullptr },{ st_float, nullptr },{ st_char, nullptr },{ st_string, nullptr } } },
	{ st_char,{ { st_integer, nullptr },{ st_float, nullptr },{ st_char, nullptr },{ st_string, nullptr } } },
	{ st_string,{ { st_integer, nullptr },{ st_float, nullptr },{ st_char, nullptr },{ st_string, nullptr } } }
};

const std::map<std::shared_ptr<_nodes::Node>, std::map<std::shared_ptr<_nodes::Node>, std::shared_ptr<_nodes::Node>>> type_table_logical =
{
	{ st_integer,{ { st_integer, st_integer },{ st_float, nullptr },{ st_char, nullptr },{ st_string, nullptr } } },
	{ st_float,{ { st_integer, nullptr },{ st_float, st_float },{ st_char, nullptr },{ st_string, nullptr } } },
	{ st_char,{ { st_integer, nullptr },{ st_float, nullptr },{ st_char, st_char },{ st_string, nullptr } } },
	{ st_string,{ { st_integer, nullptr },{ st_float, nullptr },{ st_char, nullptr },{ st_string, st_string } } }
};

const std::map<std::shared_ptr<_nodes::Node>, std::map<std::shared_ptr<_nodes::Node>, std::shared_ptr<_nodes::Node>>> type_table_relational =
{
	{ st_integer,{ { st_integer, st_integer },{ st_float, st_integer },{ st_char, nullptr },{ st_string, nullptr } } },
	{ st_float,{ { st_integer, st_integer },{ st_float, st_integer },{ st_char, nullptr },{ st_string, nullptr } } },
	{ st_char,{ { st_integer, nullptr },{ st_float, nullptr },{ st_char, st_integer },{ st_string, nullptr } } },
	{ st_string,{ { st_integer, nullptr },{ st_float, nullptr },{ st_char, nullptr },{ st_string, st_integer } } }
};

const std::map<std::shared_ptr<_nodes::Node>, std::shared_ptr<_nodes::Node>> type_table_unary_minus = 
{
	{ st_integer, st_integer}, {st_float, st_float}, {st_char, nullptr}, {st_string, nullptr}
};