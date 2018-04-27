#include "lexer.h"

Lexer::Lexer()
{
	pos_col = 1;
	pos_line = 1;
	do_read = true;
	state_ = States::whitespace;
	symbol = ' ';
}

Lexer::Lexer(std::string file) : Lexer()
{
	fopen_s(&file_, file.c_str(), "r");
}

Lexer::~Lexer()
{
	fclose(file_);
}

Token Lexer::current()
{
	return last;
}

Token Lexer::next()
{
	Token t;
	current_.clear();

	int temp_int;
	std::string temp_string;
	double temp_float;
	
	while (!feof(file_))
	{
		temp_int = 0; //Trying to fix scanf bug

		if (do_read)
			temp_int = fscanf_s(file_, "%c", &symbol);
		else
			do_read = true;

		if (temp_int == -1)
			symbol = ' ';	//Trying to fix scanf bug

		symbol = tolower(symbol);

		state_ = table[state_][symbol];
		switch (state_)
		{

		case unknown_symbol:
			throw LexerError("Unknown symbol", pos_line, pos_col);
			break;

		case unexpected_line_end:
			throw LexerError("Unexpected end of line", pos_line, pos_col);
			break;

		case comment_multiline_legacy_start:
			pos_col++;
			current_.clear();

		case whitespace:
		case comment_multiline_bracket_start:
		case dollar_hex_start:	//We do not need to push a dollar symbol
		case string_start:		//We do not need to push a quote symbol
		case asterisk_end_legacy_comment:
		case comment_multiline_bracket_read:
		case comment_multiline_legacy_read:
		case maybe_quote_symbol_2:
		case maybe_quote_symbol_3:
		case comment_multiline_bracket_end:
		case comment_multiline_legacy_end:
			pos_col++;
			break;
		
		case line_end:
		case comment_multiline_legacy_line_end:
		case comment_bracket_line_end:
			pos_line++;
			pos_col = 1;
			break;

		case semicolon_open:	//This may be legacy comment or just a separator, but we push for now
		case read_operator:
		case sharp_char_start:	//We DO need to push a char symbol
		case read_int_num:
		case read_separator:
		case read_id:
		case sharp_char_read:
		case dollar_hex_read:
		case string_read:
		case maybe_quote_symbol_1:	//We are pushing quote symbol for now, will delete it later if that meant end of a string
		case read_real_num_start:
		case read_scale_factor_start: //We'll push 'e' symbol, it will be useful later
		case read_real_num:
		case read_scale_factor_read:
		case read_scale_factor_continue:
			current_.push_back(symbol);
			pos_col++;
			break;

		case string_end:
			current_.pop_back();	//We have pushed quote symbol last time
			do_read = false;
			last = Token(Types::string_type, pos_line, pos_col, current_);
			return last;

		case int_num_end:
			do_read = false;
			last = Token(Types::int_type, pos_line, pos_col, current_);
			return last;

		case read_id_end:
			do_read = false;
			last = Token(Types::id_type, pos_line, pos_col, current_);
			return last;

		case read_separator_end:
			do_read = false;
			last = make_or_throw(Types::separator_type);
			return last;

		case read_operator_end:
			do_read = false;
			last = make_or_throw(Types::operator_type);
			return last;

		case sharp_char_end:	//We transform it in place
			temp_int = current_.rfind('#');
			//TODO check this
			temp_string = current_.substr(temp_int + 1, current_.length() - temp_int - 1);
			current_.erase(temp_int, current_.length() - temp_int);

			temp_int = stoi(temp_string);
			if (temp_int < 1 || temp_int > 255)
				throw LexerError("Bad char code", pos_line, pos_col);

			current_.push_back((char)temp_int);
			do_read = false;
			last = Token(Types::char_type, pos_line, pos_col, current_);
			return last;

		case string_with_char_start_string:	//TODO FIX
			temp_int = current_.rfind('#');
			temp_string = current_.substr(temp_int + 1, current_.length() - temp_int - 1);
			current_.erase(temp_int, current_.length() - temp_int);
			temp_int = stoi(temp_string);
			if (temp_int < 1 || temp_int > 255)
				throw LexerError("Bad char code", pos_line, pos_col);
			current_.push_back((char)temp_int);
			pos_col++;
			break;

		case dollar_hex_end:
			temp_int = stoi(current_, (size_t*)0, 16);
			current_.clear();
			current_ = std::to_string(temp_int);
			do_read = false;
			last = Token(Types::int_type, pos_line, pos_col, current_);
			return last;

		case string_with_char_start_char:
			current_.pop_back();	//We have to delete last quote (because we pushed it in)
			current_.push_back(symbol);	//Everything else goes as usual
			pos_col++;
			break;

		case real_num_end:
			temp_int = current_.rfind('e');
			if (temp_int != std::string::npos)	//Looks like we need to apply a scale factor
			{
				temp_string = current_.substr(temp_int + 1, current_.length() - temp_int - 1);
				current_.erase(temp_int, current_.length() - temp_int);
				temp_int = std::stoi(temp_string);
				temp_float = std::stod(current_);
				if (temp_int > 0)
					for (int i = 0; i < temp_int; i++, temp_float *= 10);
				else
					for (int i = 0; i < abs(temp_int); i++, temp_float /= 10);
				current_.clear();
				current_ = std::to_string(temp_float);
			}
			do_read = false;
			last = Token(Types::float_type, pos_line, pos_col, current_); //TODO we need to push there an original string, not a remade value
			return last;	

		}

	}
	//TODO make more error states
	last = Token(Types::end_of_file, pos_line, pos_col);

	return last;
}

Token Lexer::make_or_throw(Types type)
{
	Token t(type, pos_line, pos_col, current_);
	if (t.type == Types::unknown_character)		//We thought that we read an operator, but it was something bad
		switch (type)
		{
		case Types::operator_type:
			throw LexerError("Unknown symbol while reading an operator", pos_line, pos_col);
		case Types::separator_type:
			throw LexerError("Unknown symbol while reading an separator", pos_line, pos_col);
		}
		
	return t;
}


