#pragma once
#include <string>
#include <map>
#include <memory>
#include "node.h"

const std::shared_ptr<_nodes::Node> st_integer = _nodes::Type_integer::Instance();
const std::shared_ptr<_nodes::Node> st_float = _nodes::Type_float::Instance();
const std::shared_ptr<_nodes::Node> st_char = _nodes::Type_char::Instance();
const std::shared_ptr<_nodes::Node> st_string = _nodes::Type_string::Instance();

const std::string INTEGER = "integer";
const std::string FLOAT = "real";
const std::string CHAR = "char";
const std::string STRING = "string";

class Symbol_table
{
public:
	enum Field
	{
		f_constants,
		f_types,
		f_variables,
		f_functions,
		f_procedures
	};

	std::map<std::string, std::shared_ptr<_nodes::Node>> constants;
	std::map<std::string, std::shared_ptr<_nodes::Node>> types;
	std::map<std::string, std::shared_ptr<_nodes::Node>> variables;
	std::map<std::string, std::shared_ptr<_nodes::Node>> functions;
	std::map<std::string, std::shared_ptr<_nodes::Node>> procedures;

	Symbol_table() 
	{
	
	}

	void push(Field field, std::string key, std::shared_ptr<_nodes::Node> node)
	{
		switch(field)
		{
		case Field::f_constants:
			insert(&constants, key, node);
			break;
		case Field::f_types:
			insert(&types, key, node);
			break;
		case Field::f_variables:
			insert(&variables, key, node);
			break;
		case Field::f_functions:
			insert(&functions, key, node);
			break;
		case Field::f_procedures:
			insert(&procedures, key, node);
			break;
		}
	}

	std::shared_ptr<_nodes::Node> get(Field field, std::string key)
	{
		switch (field)
		{
		case Field::f_constants:
			return find(constants, key);
		case Field::f_types:
			return find(types, key);
		case Field::f_variables:
			return find(variables, key);
		case Field::f_functions:
			return find(functions, key);
		case Field::f_procedures:
			return find(procedures, key);
		}
		throw SemanticError("Code error :: Unexpected route in Symbol_table::get", 0, 0);
	}

private:
	std::shared_ptr<_nodes::Node> find(std::map<std::string, std::shared_ptr<_nodes::Node>> _map, std::string _key)
	{
		if (_map.find(_key) != _map.end())
			return _map[_key];
		else
			return nullptr;
	}

	void insert(std::map<std::string, std::shared_ptr<_nodes::Node>>* _map, std::string _key, std::shared_ptr<_nodes::Node> _node)
	{
		_map->insert(std::pair<std::string, std::shared_ptr<_nodes::Node>>(_key, _node));
	}

};