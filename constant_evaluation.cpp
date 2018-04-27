#include "constant_evaluation.h"

std::shared_ptr<_nodes::Node> operator+(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right)
{
	std::shared_ptr<_nodes::Literal> left = std::static_pointer_cast<_nodes::Literal>(_left);
	std::shared_ptr<_nodes::Literal> right = std::static_pointer_cast<_nodes::Literal>(_right);

	std::string ltype = std::static_pointer_cast<_nodes::Type>(left->ltype)->basic_type;
	std::string rtype = std::static_pointer_cast<_nodes::Type>(right->ltype)->basic_type;

	Token t;

	if (ltype == INTEGER)
	{
		if (rtype == INTEGER)
		{
			t.value = int(left->value) + int(right->value);
			t.type = Types::int_type;
			return std::make_shared<_nodes::Literal>(t, st_integer);
		}
		if (rtype == FLOAT)
		{
			t.value = int(left->value) + double(right->value);
			t.type = Types::float_type;
			return std::make_shared<_nodes::Literal>(t, st_float);
		}
		if (rtype == CHAR || rtype == STRING)
			throw SemanticError("Semantic error :: unacceptable operands in addition operation", left->t.pos_line, left->t.pos_col);
	}

	if (ltype == FLOAT)
	{
		if (rtype == INTEGER)
		{
			t.value = double(left->value) + int(right->value);
			t.type = Types::float_type;
			return std::make_shared<_nodes::Literal>(t, st_float);
		}
		if (rtype == FLOAT)
		{
			t.value = double(left->value) + double(right->value);
			t.type = Types::float_type;
			return std::make_shared<_nodes::Literal>(t, st_float);
		}
		if (rtype ==CHAR || rtype == STRING)
			throw SemanticError("Semantic error :: unacceptable operands in addition operation", left->t.pos_line, left->t.pos_col);
	}

	if (ltype == CHAR || ltype == STRING)
	{
		if (rtype == INTEGER || rtype == FLOAT)
		{
			throw SemanticError("Semantic error :: unacceptable operands in addition operation", left->t.pos_line, left->t.pos_col);
		}
		if (rtype == FLOAT || rtype == STRING)
		{
			t.value = _strdup(std::string(left->value).append(std::string(right->value)).c_str());
			t.type = Types::string_type;
			return std::make_shared<_nodes::Literal>(t, st_string);
		}
	}

	throw SemanticError("Code error :: Unexpected route in operator+ (const_eval)", 0, 0);
}

std::shared_ptr<_nodes::Node> operator-(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right)
{
	std::shared_ptr<_nodes::Literal> left = std::static_pointer_cast<_nodes::Literal>(_left);
	std::shared_ptr<_nodes::Literal> right = std::static_pointer_cast<_nodes::Literal>(_right);

	std::string ltype = std::static_pointer_cast<_nodes::Type>(left->ltype)->basic_type;
	std::string rtype = std::static_pointer_cast<_nodes::Type>(right->ltype)->basic_type;

	Token t;

	if (ltype == INTEGER)
	{
		if (rtype == INTEGER)
		{
			t.value = int(left->value) - int(right->value);
			t.type = Types::int_type;
			return std::make_shared<_nodes::Literal>(t, st_integer);
		}
		if (rtype == FLOAT)
		{
			t.value = int(left->value) - double(right->value);
			t.type = Types::float_type;
			return std::make_shared<_nodes::Literal>(t, st_float);
		}
		if (rtype == CHAR || rtype == STRING)
			throw SemanticError("Semantic error :: unacceptable operands in addition operation", left->t.pos_line, left->t.pos_col);
	}

	if (ltype == FLOAT)
	{
		if (rtype == INTEGER)
		{
			t.value = double(left->value) - int(right->value);
			t.type = Types::float_type;
			return std::make_shared<_nodes::Literal>(t, st_float);
		}
		if (rtype == FLOAT)
		{
			t.value = double(left->value) - double(right->value);
			t.type = Types::float_type;
			return std::make_shared<_nodes::Literal>(t, st_float);
		}
		if (rtype == CHAR || rtype == STRING)
			throw SemanticError("Semantic error :: unacceptable operands in addition operation", left->t.pos_line, left->t.pos_col);
	}

	if (ltype == CHAR || ltype == STRING)
	{
		throw SemanticError("Semantic error :: unacceptable operands in substitution operation", left->t.pos_line, left->t.pos_col);
	}

	throw SemanticError("Code error :: Unexpected route in operator- (const_eval)", 0, 0);
}

std::shared_ptr<_nodes::Node> operator/(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right)
{
	std::shared_ptr<_nodes::Literal> left = std::static_pointer_cast<_nodes::Literal>(_left);
	std::shared_ptr<_nodes::Literal> right = std::static_pointer_cast<_nodes::Literal>(_right);

	std::string ltype = std::static_pointer_cast<_nodes::Type>(left->ltype)->basic_type;
	std::string rtype = std::static_pointer_cast<_nodes::Type>(right->ltype)->basic_type;

	Token t;

	if (ltype == INTEGER)
	{
		if (rtype == INTEGER)
		{
			if (int(right->value) == 0)
				throw SemanticError("Semantic error :: Division by zero", right->t.pos_line, right->t.pos_col);

			t.value = double(int(left->value) / int(right->value));
			t.type = Types::float_type;
			return std::make_shared<_nodes::Literal>(t, st_float);
		}
		if (rtype == FLOAT)
		{
			if (double(right->value) == 0)
				throw SemanticError("Semantic error :: Division by zero", right->t.pos_line, right->t.pos_col);

			t.value = int(left->value) / double(right->value);
			t.type = Types::float_type;
			return std::make_shared<_nodes::Literal>(t, st_float);
		}
		if (rtype == CHAR || rtype == STRING)
			throw SemanticError("Semantic error :: unacceptable operands in division operation", left->t.pos_line, left->t.pos_col);
	}

	if (ltype == FLOAT)
	{
		if (rtype == INTEGER)
		{
			if (int(right->value) == 0)
				throw SemanticError("Semantic error :: Division by zero", right->t.pos_line, right->t.pos_col);

			t.value = double(left->value) / int(right->value);
			t.type = Types::float_type;
			return std::make_shared<_nodes::Literal>(t, st_float);
		}
		if (rtype == FLOAT)
		{
			if (double(right->value) == 0)
				throw SemanticError("Semantic error :: Division by zero", right->t.pos_line, right->t.pos_col);

			t.value = double(left->value) / double(right->value);
			t.type = Types::float_type;
			return std::make_shared<_nodes::Literal>(t, st_float);
		}
		if (rtype == CHAR || rtype == STRING)
			throw SemanticError("Semantic error :: unacceptable operands in division operation", left->t.pos_line, left->t.pos_col);
	}

	if (ltype == CHAR || ltype == STRING)
	{
		throw SemanticError("Semantic error :: unacceptable operands in division operation", left->t.pos_line, left->t.pos_col);
	}

	throw SemanticError("Code error :: Unexpected route in operator/ (const_eval)", 0, 0);
}

std::shared_ptr<_nodes::Node> operator*(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right)
{
	std::shared_ptr<_nodes::Literal> left = std::static_pointer_cast<_nodes::Literal>(_left);
	std::shared_ptr<_nodes::Literal> right = std::static_pointer_cast<_nodes::Literal>(_right);

	std::string ltype = std::static_pointer_cast<_nodes::Type>(left->ltype)->basic_type;
	std::string rtype = std::static_pointer_cast<_nodes::Type>(right->ltype)->basic_type;

	Token t;

	if (ltype == INTEGER)
	{
		if (rtype == INTEGER)
		{
			t.value = int(left->value) * int(right->value);
			t.type = Types::int_type;
			return std::make_shared<_nodes::Literal>(t, st_integer);
		}
		if (rtype == FLOAT)
		{
			t.value = int(left->value) * double(right->value);
			t.type = Types::float_type;
			return std::make_shared<_nodes::Literal>(t, st_float);
		}
		if (rtype == CHAR || rtype == STRING)
			throw SemanticError("Semantic error :: unacceptable operands in multiplication operation", left->t.pos_line, left->t.pos_col);
	}

	if (ltype == FLOAT)
	{
		if (rtype == INTEGER)
		{
			t.value = double(left->value) * int(right->value);
			t.type = Types::float_type;
			return std::make_shared<_nodes::Literal>(t, st_float);
		}
		if (rtype == FLOAT)
		{
			t.value = double(left->value) * double(right->value);
			t.type = Types::float_type;
			return std::make_shared<_nodes::Literal>(t, st_float);
		}
		if (rtype == CHAR || rtype == STRING)
			throw SemanticError("Semantic error :: unacceptable operands in multiplication operation", left->t.pos_line, left->t.pos_col);
	}

	if (ltype == CHAR || ltype == STRING)
	{
		if (rtype == INTEGER || rtype == FLOAT)
		{
			throw SemanticError("Semantic error :: unacceptable operands in multiplication operation", left->t.pos_line, left->t.pos_col);
		}
		if (rtype == FLOAT || rtype == STRING)
		{
			t.value = _strdup(std::string(left->value).append(std::string(right->value)).c_str());
			t.type = Types::string_type;
			return std::make_shared<_nodes::Literal>(t, st_string);
		}
	}

	throw SemanticError("Code error :: Unexpected route in operator* (const_eval)", 0, 0);
}

std::shared_ptr<_nodes::Node> operator_div(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right)
{
	std::shared_ptr<_nodes::Literal> left = std::static_pointer_cast<_nodes::Literal>(_left);
	std::shared_ptr<_nodes::Literal> right = std::static_pointer_cast<_nodes::Literal>(_right);

	std::string ltype = std::static_pointer_cast<_nodes::Type>(left->ltype)->basic_type;
	std::string rtype = std::static_pointer_cast<_nodes::Type>(right->ltype)->basic_type;

	Token t;

	if (ltype == INTEGER)
	{
		if (rtype == INTEGER)
		{
			t.value = int(int(left->value) / int(right->value));
			t.type = Types::int_type;
			return std::make_shared<_nodes::Literal>(t, st_integer);
		}
		else
			throw SemanticError("Semantic error :: unacceptable operands in division operation", left->t.pos_line, left->t.pos_col);
	}

	if (ltype == FLOAT)
	{
		throw SemanticError("Semantic error :: unacceptable operands in division operation", left->t.pos_line, left->t.pos_col);
	}

	if (ltype == CHAR || ltype == STRING)
	{
		throw SemanticError("Semantic error :: unacceptable operands in division operation", left->t.pos_line, left->t.pos_col);
	}

	throw SemanticError("Code error :: Unexpected route in operator+ (const_eval)", 0, 0);
}

std::shared_ptr<_nodes::Node> operator_mod(std::shared_ptr<_nodes::Node> _left, std::shared_ptr<_nodes::Node> _right)
{
	std::shared_ptr<_nodes::Literal> left = std::static_pointer_cast<_nodes::Literal>(_left);
	std::shared_ptr<_nodes::Literal> right = std::static_pointer_cast<_nodes::Literal>(_right);

	std::string ltype = std::static_pointer_cast<_nodes::Type>(left->ltype)->basic_type;
	std::string rtype = std::static_pointer_cast<_nodes::Type>(right->ltype)->basic_type;

	Token t;

	if (ltype == INTEGER)
	{
		if (rtype == INTEGER)
		{
			t.value = int(left->value) % int(right->value);
			t.type = Types::int_type;
			return std::make_shared<_nodes::Literal>(t, st_integer);
		}
		else
			throw SemanticError("Semantic error :: unacceptable operands in remainder operation", left->t.pos_line, left->t.pos_col);
	}

	if (ltype == FLOAT)
	{
		throw SemanticError("Semantic error :: unacceptable operands in remainder operation", left->t.pos_line, left->t.pos_col);
	}

	if (ltype == CHAR || ltype == STRING)
	{
		throw SemanticError("Semantic error :: unacceptable operands in remainder operation", left->t.pos_line, left->t.pos_col);
	}

	throw SemanticError("Code error :: Unexpected route in operator mod (const_eval)", 0, 0);
}

std::shared_ptr<_nodes::Node> operator_unary_minus(std::shared_ptr<_nodes::Node> _left)
{
	std::shared_ptr<_nodes::Literal> left = std::static_pointer_cast<_nodes::Literal>(_left);
	std::string ltype = std::static_pointer_cast<_nodes::Type>(left->ltype)->basic_type;

	Token t;

	if (ltype == INTEGER)
	{
		t.value = -(int(left->value));
		t.type = Types::int_type;
		return std::make_shared<_nodes::Literal>(t, st_integer);
	}

	if (ltype == FLOAT)
	{
		t.value = -(double(left->value));
		t.type = Types::float_type;
		return std::make_shared<_nodes::Literal>(t, st_float);
	}

	if (ltype == CHAR || ltype == STRING)
	{
		throw SemanticError("Semantic error :: unacceptable operands in unary_minus operation", left->t.pos_line, left->t.pos_col);
	}

	throw SemanticError("Code error :: Unexpected route in operator+ (const_eval)", 0, 0);
}

std::shared_ptr<_nodes::Node> get_type(std::shared_ptr<_nodes::Node> node)
{
	if (node == nullptr)
		return nullptr;

	std::shared_ptr<_nodes::Node> ltype, rtype, newtype;
	std::shared_ptr<_nodes::Typecast> caster;
	switch (node->type)
	{
	case Node_types::node_literal:
		return std::static_pointer_cast<_nodes::Literal>(node)->ltype;
	case Node_types::node_constant:
		return std::static_pointer_cast<_nodes::Constant>(node)->ctype;
	case Node_types::node_variable:
		return std::static_pointer_cast<_nodes::Variable>(node)->vtype;
	case Node_types::node_typecast:
		return std::static_pointer_cast<_nodes::Typecast>(node)->cast_type;
	case Node_types::node_function_call:
		return std::static_pointer_cast<_nodes::Function_declaration>(std::static_pointer_cast<_nodes::Function_call>(node)->function)->ftype;
	case Node_types::node_operator:
		ltype = get_type(node->child[0]);
		rtype = get_type(node->child[1]);
		switch (Operators(node->t.value))
		{
		case Operators::addition:
		case Operators::substraction:
		case Operators::multiplication:
			newtype = type_table_addition.find(ltype)->second.find(rtype)->second;
			break;
		case Operators::division:
			newtype = type_table_division.find(ltype)->second.find(rtype)->second;
			break;
		case Operators::integer_division:
		case Operators::remainder_operator:
			newtype = type_table_remainder.find(ltype)->second.find(rtype)->second;
			break;
		case Operators::bitwise_and:
		case Operators::bitwise_negation:
		case Operators::bitwise_or:
		case Operators::bitwise_shift_to_the_left:
		case Operators::bitwise_shift_to_the_right:
		case Operators::bitwise_xor:
			newtype = type_table_logical.find(ltype)->second.find(rtype)->second;
			break;
		case Operators::equal:
		case Operators::not_equal:
		case Operators::strictly_less_than:
		case Operators::strictly_greater_than:
		case Operators::less_than_or_equal:
		case Operators::greater_than_or_equal:
			newtype = type_table_relational.find(ltype)->second.find(rtype)->second;
			break;
		case Operators::unary_minus:
			newtype = type_table_unary_minus.find(ltype)->second;
			if (newtype == nullptr)
				throw SemanticError("Semantic error :: unacceptable operands in expression", node->t.pos_line, node->t.pos_col);
			return newtype;
		}
		if (newtype == nullptr)
			throw SemanticError("Semantic error :: unacceptable operands in expression", node->t.pos_line, node->t.pos_col);
		else
		{
			if (ltype == st_integer && rtype == st_float)
			{
				caster = std::make_shared<_nodes::Typecast>();
				caster->cast_type = st_float;
				caster->expression = node->child[0];
				node->child[0] = caster;
			}
			else if (rtype == st_integer && ltype == st_float)
			{
				caster = std::make_shared<_nodes::Typecast>();
				caster->cast_type = st_float;
				caster->expression = node->child[1];
				node->child[1] = caster;
			}
			return newtype;
		}
	}
	throw SemanticError("Code error :: Unexpected route in get_type (const_eval)", 0, 0);
}
