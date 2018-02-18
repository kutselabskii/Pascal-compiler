#include "lexer.h"

Lexer::Lexer()
{
	pos_col = 1;
	pos_line = 1;
}

Lexer::Lexer(std::string file) : Lexer()
{
	file_.open(file.c_str(), 'r');
}

Lexer::~Lexer()
{
	file_.close();
}

Token Lexer::next()
{
	char symbol;
	current_.clear();

	bool do_read = true;

	//?
	States last;
	//?

	while (!file_.eof())
	{
		if (do_read)
			file_ >> symbol;
		else
			do_read = true;

		//?
		last = state_;
		//?

		state_ = table[state_][symbol < 0 ? 129 : tolower(symbol)];
		switch (state_)
		{

		case unknown_symbol:
			throw LexerError("Unknown symbol", pos_line, pos_col);

		case unexpected_line_end:
			throw LexerError("Unexpected end of line", pos_line, pos_col);

		case whitespace:
		case comment_multiline_bracket_start:
		case sharp_char_start:	//We do not need to push a sharp symbol
		case dollar_hex_start:	//We do not need to push a dollar symbol
		case string_start:		//We do not need to push a quote symbol
		case asterisk_end_legacy_comment:
		case comment_multiline_bracket_read:
		case comment_multiline_legacy_start:
		case comment_multiline_legacy_read:
			pos_col++;

		case comment_multiline_bracket_end:
		case comment_multiline_legacy_end:
			do_read = false;
		
		case line_end:
		case comment_multiline_legacy_line_end:
		case comment_bracket_line_end:
			pos_line++;
			pos_col = 1;

		case semicolon_open:	//This may be legacy comment or just a separator, but we push for now
		case read_operator:
		case read_int_num:
		case read_separator:
		case read_id:
		case sharp_char_read:
		case dollar_hex_read:
		case string_read:
			current_.push_back(symbol);
			pos_col++;

		case string_end:
			do_read = false;
			return Token(Types::string_type, pos_line, pos_col, current_);

		case int_num_end:
			do_read = false;
			return Token(Types::int_type, pos_line, pos_col, current_);

		case read_id_end:
			do_read = false;
			return Token(Types::id_type, pos_line, pos_col, current_);

		case read_operator_end:
			Token t(Types::operator_type, pos_line, pos_col, current_);
			if (t.type == Types::unknown_character)		//We thought that we read an operator, but it was something bad
				throw LexerError("Unknown symbol while reading an operator", pos_line, pos_col);

		//27 READ SEPARATOR END
		}

	}

	Token t(Types::end_of_file);

	return t;
}
