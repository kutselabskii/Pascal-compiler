#include "generator.h"

std::string Asm_element::print()
{
	return std::string();
}

std::shared_ptr<Asm_code> Generate_code(std::shared_ptr<_nodes::Node> node)
{
	generate(node);
	return asm_code;
}

bool generate(std::shared_ptr<_nodes::Node> node)
{
	bool res, resr;
	std::string endlabel, elselabel, label;

	if (node == nullptr)
		return false;
	switch (node->type)
	{
	case Node_types::node_program:
		//generate(std::static_pointer_cast<_nodes::Program>(node)->program_heading);	//Not needed
		return generate(std::static_pointer_cast<_nodes::Program>(node)->block);	//doesnt matter

	case Node_types::node_block:
		//generate(std::static_pointer_cast<_nodes::Block>(node)->declaration_part);
		asm_code->symbol_table_stack.push_back(std::static_pointer_cast<_nodes::Block>(node)->symbol_table);
		asm_code->add_command(Asm_commands::push, Registers::EBP);
		asm_code->add_command(Asm_commands::mov, Registers::EBP, Registers::ESP);
		asm_code->add_command("sub esp, " + std::to_string(get_max_offset(asm_code->symbol_table_stack.back())));
		asm_code->add_command("; memory_allocation\n");
		res = generate(std::static_pointer_cast<_nodes::Block>(node)->statement_part);
		asm_code->symbol_table_stack.pop_back();
		return res;	//doesnt matter
	case Node_types::node_statement_part:
		return generate(std::static_pointer_cast<_nodes::Statement_part>(node)->sequence);
	case Node_types::node_statement_sequence:
		for (unsigned int i = 0; i < std::static_pointer_cast<_nodes::Statement_sequence>(node)->values.size(); i++)
			generate(std::static_pointer_cast<_nodes::Statement_sequence>(node)->values[i]);
		return false;	//doesnt matter
	case Node_types::node_assignment_statement:
		res = generate(std::static_pointer_cast<_nodes::Assignment_statement>(node)->expression);
		generate(std::static_pointer_cast<_nodes::Assignment_statement>(node)->variable);

		asm_code->add_command(Asm_commands::pop, EAX);	//Should be address
		if (res)
		{
			asm_code->add_command(Asm_commands::pop, Registers::EBX);
			asm_code->add_command("mov ebx, dword ptr[ebx]");
		}
		else
			asm_code->add_command(Asm_commands::pop, EBX);
		asm_code->add_command("mov dword ptr[eax], ebx");
		asm_code->add_command("; assignment_statement\n");
		return false;	//doesnt matter
	case Node_types::node_variable:
		asm_code->add_command("lea eax, dword ptr [ebp - " + std::to_string(get_offset(asm_code->symbol_table_stack.back(), node)) + "]");
		asm_code->add_command(Asm_commands::push, Registers::EAX);
		asm_code->add_command("; pushing variable address\n");
		return true;	//matters
	case Node_types::node_literal:
		if (node->t.type == Types::int_type)
			asm_code->add_command(Asm_commands::push, int(node->t.value));
		asm_code->add_command("; pushing int constant value\n");
		return false;	//matters
	case Node_types::node_write_statement:
		res = generate(std::static_pointer_cast<_nodes::Write_statement>(node)->expression);
		
		if (res)
		{
			asm_code->add_command(Asm_commands::pop, Registers::EAX);
			asm_code->add_command("push dword ptr [eax]");
		}
		
		asm_code->add_command("push offset __str@0");
		asm_code->add_command("call crt_printf");
		asm_code->add_command("; write_statement\n");
		return false;
	case Node_types::node_if_statement:
		res = generate(std::static_pointer_cast<_nodes::If_statement>(node)->expression);
		asm_code->add_command(Asm_commands::pop, Registers::EAX);
		if (res == true)
			asm_code->add_command("mov eax, dword ptr[eax]");
		asm_code->add_command(Asm_commands::test, Registers::EAX, Registers::EAX);
		elselabel = "elselabel" + std::to_string(asm_code->labelcount);
		asm_code->labelcount++;
		endlabel = "endlabel" + std::to_string(asm_code->labelcount);
		asm_code->labelcount++;
		asm_code->add_command("jz " + elselabel);
		res = generate(std::static_pointer_cast<_nodes::If_statement>(node)->then_statement);
		asm_code->add_command("jmp " + endlabel);
		asm_code->add_command(elselabel + ":");
		res = generate(std::static_pointer_cast<_nodes::If_statement>(node)->else_statement);
		asm_code->add_command(endlabel + ":");
		return false;
	case Node_types::node_while_statement:
		label = "whilelabel" + std::to_string(asm_code->labelcount);
		asm_code->labelcount++;
		endlabel = "bodylabel" + std::to_string(asm_code->labelcount);
		asm_code->labelcount++;

		asm_code->add_command("jmp " + label);
		asm_code->add_command(endlabel + ":");
		res = generate(std::static_pointer_cast<_nodes::While_statement>(node)->statement);
		asm_code->add_command(label + ":");
		res = generate(std::static_pointer_cast<_nodes::While_statement>(node)->expression);
		asm_code->add_command(Asm_commands::pop, Registers::EAX);
		if (res == true)
			asm_code->add_command("mov eax, dword ptr[eax]");
		asm_code->add_command(Asm_commands::test, EAX, EAX);
		asm_code->add_command("jnzz " + endlabel);
		return false;
		
	case Node_types::node_operator:
		res = generate(node->child[0]);
		resr = generate(node->child[1]);
		if (resr == false)
			asm_code->add_command(Asm_commands::pop, Registers::EAX);
		else
		{
			asm_code->add_command(Asm_commands::pop, Registers::EAX);
			asm_code->add_command("mov eax, dword ptr [eax]");
		}
		asm_code->add_command("; getting left child value\n");
		if (res == false)
			asm_code->add_command(Asm_commands::pop, Registers::EBX);
		else
		{
			asm_code->add_command(Asm_commands::pop, Registers::EBX);
			asm_code->add_command("mov ebx, dword ptr [ebx]");
		}
		asm_code->add_command("; getting right child value\n");
		switch (Operators(node->t.value))
		{
		case Operators::addition:
			asm_code->add_command(Asm_commands::add, Registers::EAX, Registers::EBX);
			asm_code->add_command(Asm_commands::push, Registers::EAX);
			asm_code->add_command("; addition\n");
			break;
		case Operators::substraction:
			asm_code->add_command(Asm_commands::sub, Registers::EBX, Registers::EAX);
			asm_code->add_command(Asm_commands::push, Registers::EBX);
			asm_code->add_command("; substraction\n");
			break;
		case Operators::multiplication:
			asm_code->add_command(Asm_commands::imul, Registers::EAX, Registers::EBX);
			asm_code->add_command(Asm_commands::push, Registers::EAX);
			asm_code->add_command("; multiplication\n");
			break;
		case Operators::integer_division:
			asm_code->add_command(Asm_commands::mov, Registers::ECX, Registers::EAX);
			asm_code->add_command(Asm_commands::mov, Registers::EAX, Registers::EBX);
			asm_code->add_command(Asm_commands::mov, Registers::EBX, Registers::ECX);

			asm_code->add_command(Asm_commands::cdq);
			asm_code->add_command(Asm_commands::idiv, Registers::EBX);
			asm_code->add_command(Asm_commands::push, Registers::EAX);
			asm_code->add_command("; integer division\n");
			break;
		case Operators::remainder_operator:
			asm_code->add_command(Asm_commands::mov, Registers::ECX, Registers::EAX);
			asm_code->add_command(Asm_commands::mov, Registers::EAX, Registers::EBX);
			asm_code->add_command(Asm_commands::mov, Registers::EBX, Registers::ECX);

			asm_code->add_command(Asm_commands::cdq);
			asm_code->add_command(Asm_commands::idiv, Registers::EBX);
			asm_code->add_command(Asm_commands::push, Registers::EDX);
			asm_code->add_command("; remainder\n");
			break;
		case Operators::equal:
			asm_code->add_command(Asm_commands::cmp, Registers::EAX, Registers::EBX);
			asm_code->add_command("sete al");
			asm_code->add_command("movsx eax, al");
			asm_code->add_command(Asm_commands::push, Registers::EAX);
			asm_code->add_command("; operation_equal\n");
			break;
		case Operators::not_equal:
			asm_code->add_command(Asm_commands::cmp, Registers::EAX, Registers::EBX);
			asm_code->add_command("setne al");
			asm_code->add_command("movsx eax, al");
			asm_code->add_command(Asm_commands::push, Registers::EAX);
			asm_code->add_command("; operation not_equal\n");
			break;
		case Operators::less_than_or_equal:
			asm_code->add_command(Asm_commands::mov, Registers::ECX, Registers::EAX);
			asm_code->add_command(Asm_commands::mov, Registers::EAX, Registers::EBX);
			asm_code->add_command(Asm_commands::mov, Registers::EBX, Registers::ECX);

			asm_code->add_command(Asm_commands::cmp, Registers::EAX, Registers::EBX);
			asm_code->add_command("setle al");
			asm_code->add_command("movsx eax, al");
			asm_code->add_command(Asm_commands::push, Registers::EAX);
			asm_code->add_command("; operation <=\n");
			break;
		case Operators::strictly_less_than:
			asm_code->add_command(Asm_commands::mov, Registers::ECX, Registers::EAX);
			asm_code->add_command(Asm_commands::mov, Registers::EAX, Registers::EBX);
			asm_code->add_command(Asm_commands::mov, Registers::EBX, Registers::ECX);

			asm_code->add_command(Asm_commands::cmp, Registers::EAX, Registers::EBX);
			asm_code->add_command("setl al");
			asm_code->add_command("movsx eax, al");
			asm_code->add_command(Asm_commands::push, Registers::EAX);
			asm_code->add_command("; operation <\n");
			break;
		case Operators::greater_than_or_equal:
			asm_code->add_command(Asm_commands::mov, Registers::ECX, Registers::EAX);
			asm_code->add_command(Asm_commands::mov, Registers::EAX, Registers::EBX);
			asm_code->add_command(Asm_commands::mov, Registers::EBX, Registers::ECX);

			asm_code->add_command(Asm_commands::cmp, Registers::EAX, Registers::EBX);
			asm_code->add_command("setge al");
			asm_code->add_command("movsx eax, al");
			asm_code->add_command(Asm_commands::push, Registers::EAX);
			asm_code->add_command("; operation >=\n");
			break;
		case Operators::strictly_greater_than:
			asm_code->add_command(Asm_commands::mov, Registers::ECX, Registers::EAX);
			asm_code->add_command(Asm_commands::mov, Registers::EAX, Registers::EBX);
			asm_code->add_command(Asm_commands::mov, Registers::EBX, Registers::ECX);

			asm_code->add_command(Asm_commands::cmp, Registers::EAX, Registers::EBX);
			asm_code->add_command("setg al");
			asm_code->add_command("movsx eax, al");
			asm_code->add_command(Asm_commands::push, Registers::EAX);
			asm_code->add_command("; operation >\n");
			break;
		case Operators::bitwise_and:
			asm_code->add_command(Asm_commands::and, Registers::EAX, Registers::EBX);
			asm_code->add_command(Asm_commands::push, Registers::EAX);
			asm_code->add_command("; operation bitwise and\n");
			break;
		case Operators::bitwise_or:
			asm_code->add_command(Asm_commands::or, Registers::EAX, Registers::EBX);
			asm_code->add_command(Asm_commands::push, Registers::EAX);
			asm_code->add_command("; operation bitwise and\n");
			break;
		case Operators::bitwise_xor:
			asm_code->add_command(Asm_commands::c_xor, Registers::EAX, Registers::EBX);
			asm_code->add_command(Asm_commands::push, Registers::EAX);
			asm_code->add_command("; operation bitwise and\n");
			break;
		}
		return false;
	}
	return false;
}

void Asm_code::print(std::string filename)
{
	std::ofstream f(filename);

	f << "include d:\\masm32\\include\\masm32rt.inc" << std::endl << std::endl;

	f << ".xmm" << std::endl;
	if (constants.size())
	{
		f << ".const" << std::endl;
		for (unsigned int i = 0; i < constants.size(); i++)
			f << constants[i]->print() << std::endl;
	}

	f << ".const" << std::endl;
	f << "__str@0 db 37, 100, 10, 0" << std::endl;

	f << ".code" << std::endl;

	f << "start:" << std::endl << std::endl;
	for (unsigned int i = 0; i < commands.size(); i++)
		f << commands[i] << std::endl;
	f << std::endl;

	
	//f << "call __@function0" << std::endl;
	//f << "exit" << std::endl;
	f << "exit" << std::endl;
	f << "end start" << std::endl;

	f.close();
}

Asm_element::Asm_element(Asm_commands _op_code)
{
	op_code = _op_code;
}

void Asm_code::add_constant(std::shared_ptr<Asm_constant> constant)
{
	constants.push_back(constant);
}

std::string Asm_label::print()
{
	return commands_to_string.find(op_code)->second + ":";
}


Asm_constant::Asm_constant(std::string _name, std::string _dx, std::string _content)
{
	name = _name;
	dx = _dx;
	content = _content;
}

std::string Asm_constant::print()
{
	return name + " " + dx + " " + content;
}

void Asm_code::add_command(Asm_commands com)
{
	commands.push_back(commands_to_string.find(com)->second);
}

void Asm_code::add_command(Asm_commands com, int val)
{
	commands.push_back(commands_to_string.find(com)->second + " " + std::to_string(val));
}

void Asm_code::add_command(Asm_commands com, int leftval, int rightval)
{
	commands.push_back(commands_to_string.find(com)->second + " " + std::to_string(leftval) + ", " + std::to_string(rightval));
}

void Asm_code::add_command(Asm_commands com, Registers reg)
{
	commands.push_back(commands_to_string.find(com)->second + " " + register_to_string.find(reg)->second);
}

void Asm_code::add_command(Asm_commands com, Registers reg, int val)
{
	commands.push_back(commands_to_string.find(com)->second + " " + register_to_string.find(reg)->second + ", " + std::to_string(val));
}

void Asm_code::add_command(Asm_commands com, Registers leftreg, Registers rightreg)
{
	commands.push_back(commands_to_string.find(com)->second + " " + register_to_string.find(leftreg)->second + ", " + register_to_string.find(rightreg)->second);
}

void Asm_code::add_command(std::string com)
{
	commands.push_back(com);
}

int get_offset(std::shared_ptr<Symbol_table> symbol_table, std::shared_ptr<_nodes::Node> node)
{
	int offset = 0;
	for (std::map<std::string, std::shared_ptr<_nodes::Node>>::iterator iter = symbol_table->variables.begin(); iter != symbol_table->variables.end(); iter++)
	{
		offset += std::static_pointer_cast<_nodes::Type>(std::static_pointer_cast<_nodes::Variable>(iter->second)->vtype)->byte;
		if (node == iter->second)
		{
			if (offset > 0)
				offset -= 4;
			return offset;
		}
	}

	return offset;
}

int get_max_offset(std::shared_ptr<Symbol_table> symbol_table)
{
	int offset = 0;
	for (std::map<std::string, std::shared_ptr<_nodes::Node>>::iterator iter = symbol_table->variables.begin(); iter != symbol_table->variables.end(); iter++)
		offset += std::static_pointer_cast<_nodes::Type>(std::static_pointer_cast<_nodes::Variable>(iter->second)->vtype)->byte;

	return offset;
}