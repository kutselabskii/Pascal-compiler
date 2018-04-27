#pragma once
#include <memory>
#include <vector>


const std::string NOTANAME = "@@@";

class Symbol_table;

namespace _nodes
{
	class Node
	{
	public:

		Node()
		{
			child[0] = child[1] = nullptr;
		}

		Node(Token _t)
		{
			child[0] = child[1] = nullptr;
			t = _t;
		}

		~Node() { }
		Token t;

		std::shared_ptr<Node>child[2];
		Node_types type;
	};

	class Program : public Node	//Printed
	{
	public:
		Program() : Node()
		{
			program_heading = block = nullptr;
			type = Node_types::node_program;
		}

		Program(Token _t) : Node(_t)
		{
			program_heading = block = nullptr;
			type = Node_types::node_program;
		}

		std::shared_ptr<Node> program_heading, block;
	};	//DONE

	class Program_heading : public Node	//Printed
	{
	public:
		Program_heading(Token _t) : Node (_t)
		{
			type = Node_types::node_program_heading;
			heading = std::string(_t.value);
		}

		std::string heading;
	};	//DONE

	class Block : public Node	//Printed
	{
	public:
		Block() : Node()
		{
			type = Node_types::node_block;
			declaration_part = statement_part = nullptr;
			symbol_table = nullptr;
		}

		Block(Token _t) : Node(_t)
		{
			type = Node_types::node_block;
			declaration_part = statement_part = nullptr;
			symbol_table = nullptr;
		}

		std::shared_ptr<Node> declaration_part, statement_part;
		std::shared_ptr<Symbol_table> symbol_table;
	};  //DONE

	class Declaration_part : public Node	//Printed
	{
	public:
		Declaration_part() : Node()
		{
			type = Node_types::node_declaration_part;
		}

		Declaration_part(Token _t) : Node(_t)
		{
			type = Node_types::node_declaration_part;
		}
		std::vector<std::shared_ptr<Node>> constants;
		std::vector<std::shared_ptr<Node>> variables;
		std::vector<std::shared_ptr<Node>> procedures;
		std::vector<std::shared_ptr<Node>> functions;
		std::vector<std::shared_ptr<Node>> types;
	};	 //DONE

	class Statement_part : public Node
	{
	public:
		Statement_part() : Node()
		{
			type = Node_types::node_statement_part;
		}
		std::shared_ptr<_nodes::Node> sequence;
	};  //DONE

	class Type : public Node
	{
	public:
		Type() : Node()
		{
			type = Node_types::node_type;
		}

		std::string basic_type;
		std::string name;
		int byte;
	};

	class Type_integer : public Type
	{
	public:
		static std::shared_ptr<_nodes::Type_integer> Instance()
		{
			static std::shared_ptr<_nodes::Type_integer> s = std::make_shared<_nodes::Type_integer>();
			return s;
		}

		Type_integer() : Type()
		{
			basic_type = "integer";
			byte = 4;
		}
	};

	class Type_float : public Type
	{
	public:
		static std::shared_ptr<_nodes::Type_float> Instance()
		{
			static std::shared_ptr<_nodes::Type_float> s = std::make_shared<_nodes::Type_float>();
			return s;
		}

		Type_float() : Type()
		{
			basic_type = "real";
			byte = 8;
		}
	};

	class Type_char : public Type
	{
	public:
		static std::shared_ptr<_nodes::Type_char> Instance()
		{
			static std::shared_ptr<_nodes::Type_char> s = std::make_shared<_nodes::Type_char>();
			return s;
		}

		Type_char() : Type()
		{
			basic_type = "char";
		}
	};

	class Type_string : public Type
	{
	public:
		static std::shared_ptr<_nodes::Type_string> Instance()
		{
			static std::shared_ptr<_nodes::Type_string> s = std::make_shared<_nodes::Type_string>();
			return s;
		}

		Type_string() : Type()
		{
			basic_type = "string";
		}
	};

	//					CONSTANTS
	class Constant : public Node	//Printed
	{
	public:
		Constant(std::string _name, std::shared_ptr<_nodes::Node> _value, std::shared_ptr<_nodes::Node> _type) : Node()
		{
			type = Node_types::node_constant;
			name = _name;
			value = _value;
			_type = ctype;
		}
		std::string name;
		std::shared_ptr<_nodes::Node> value;
		std::shared_ptr<_nodes::Node> ctype;
	};

	class Literal : public Node
	{
	public:
		Literal(Token _t, std::shared_ptr<_nodes::Node>_type) : Node(_t)
		{
			type = Node_types::node_literal;
			ltype = _type;
			value = _t.value;
		}

		std::shared_ptr<_nodes::Node> ltype;
		Token::Value value;
	};

	class Def_decl_part : public Node
	{
	public:
		Def_decl_part() : Node() {}
		std::vector <std::shared_ptr<_nodes::Node>> values;
	};

	class Constant_definition_part : public Def_decl_part	//Printed
	{
	public:
		Constant_definition_part() : Def_decl_part()
		{
			type = Node_types::node_constant_definition_part;
		}
	};

	class Type_definition_part : public Def_decl_part	//Printed
	{
	public:
		Type_definition_part() : Def_decl_part()
		{
			type = Node_types::node_type_definition_part;
		}
	};

	class Variable : public Node
	{
	public:
		Variable(std::string _name, std::shared_ptr<_nodes::Node> _type, std::shared_ptr<_nodes::Node> _value=nullptr) : Node()
		{
			type = Node_types::node_variable;
			name = _name;
			vtype = _type;
			value = _value;
		}
		std::string name;
		std::shared_ptr<_nodes::Node> value;
		std::shared_ptr<_nodes::Node> vtype;
	};

	class Variable_declaration_part : public Def_decl_part
	{
	public:
		Variable_declaration_part() : Def_decl_part()
		{
			type = Node_types::node_variable_declaration_part;
		}
	};


	class Operator : public Node
	{
	public:
		Operator(Token _t) : Node(_t)
		{
			type = Node_types::node_operator;
		}
	};

	class Statement_sequence : public Def_decl_part
	{
	public:
		Statement_sequence() : Def_decl_part()
		{
			type = Node_types::node_statement_sequence;
		}
	};  //DONE

	class Assignment_statement : public Node
	{
	public:
		Assignment_statement() : Node()
		{
			type = Node_types::node_assignment_statement;
			variable = expression = nullptr;
		}
		std::shared_ptr<_nodes::Node> variable, expression;
	};

	class Typecast : public Node
	{
	public:
		Typecast() : Node()
		{
			type = Node_types::node_typecast;
			cast_type = expression = nullptr;
		}

		std::shared_ptr<_nodes::Node> cast_type, expression;
	};

	class Procedure_declaration : public Node
	{
	public:
		Procedure_declaration() : Node()
		{
			type = Node_types::node_procedure_declaration;
		}
		std::shared_ptr<_nodes::Node> heading, body;
		std::shared_ptr<Symbol_table> symbol_table;
		std::string name;
	};

	class Function_declaration : public Node
	{
	public:
		Function_declaration() : Node()
		{
			type = Node_types::node_function_declaration;
		}
		std::shared_ptr<_nodes::Node> heading, body, ftype;
		std::shared_ptr<Symbol_table> symbol_table;
		std::string name;
	};

	class Function_argument_list : public Def_decl_part
	{
	public:
		Function_argument_list() : Def_decl_part()
		{
			type = Node_types::node_function_argument_list;
		}
	};

	class Function_call : public Def_decl_part
	{
	public:
		Function_call() : Def_decl_part()
		{
			type = Node_types::node_function_call;
		}

		std::shared_ptr<_nodes::Function_declaration> function;
	};

	class Procedure_statement : public Def_decl_part
	{
	public:
		Procedure_statement() : Def_decl_part()
		{
			type = Node_types::node_procedure_statement;
		}

		std::shared_ptr<_nodes::Procedure_declaration> procedure;
	};

	class While_statement : public Node
	{
	public:
		While_statement() : Node()
		{
			type = Node_types::node_while_statement;
		}
		std::shared_ptr<_nodes::Node> expression, statement;
	};

	class Repeat_statement : public Node
	{
	public:
		Repeat_statement() : Node()
		{
			type = Node_types::node_repeat_statement;
		}
		std::shared_ptr<_nodes::Node> expression, statements;
	};

	class For_statement : public Node
	{
	public:
		For_statement() : Node()
		{
			type = Node_types::node_for_statement;
			statement = variable = start_expression = finish_expression = nullptr;
			downto = false;
		}
		std::shared_ptr<_nodes::Node> statement, variable, start_expression, finish_expression;
		bool downto;
	};

	class Write_statement : public Node
	{
	public:
		Write_statement() : Node()
		{
			type = Node_types::node_write_statement;
		}
		std::shared_ptr<_nodes::Node> expression;
	};

	class If_statement : public Node
	{
	public:
		If_statement() : Node()
		{
			type = Node_types::node_if_statement;
			expression = then_statement = else_statement = nullptr;
		}
		std::shared_ptr<_nodes::Node> expression, then_statement, else_statement;
	};
}




