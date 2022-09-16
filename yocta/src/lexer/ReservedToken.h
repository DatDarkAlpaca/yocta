#pragma once
#include "LookupTable.h"

namespace yo
{
	enum class ReservedToken
	{
		T_NONE,

		// Arithmetic Operators:
		T_ADD,						//  +
		T_SUB,						//  -
		T_MULT,						//  *
		T_DIV,						//  /
		T_MOD,						//  %

		T_INCREMENT,				//  ++
		T_DECREMENT,				//  --

		// Bitwise Operators:
		T_BITNOT,					//  ~
		T_BITAND,					//  &
		T_BITOR,					//  |
		T_BITXOR,					//  ^
		T_BITSHIFT_LEFT,			//	<<
		T_BITSHIFT_RIGHT,			//	>>

		// Logical Operators:
		T_NOT,						//	!
		T_AND,						//	&&
		T_OR,						//	||

		// Assignment Operators:
		T_ASSIGN,					//  =
		T_ADD_ASSIGN,				//  +=
		T_SUB_ASSIGN,				//  -=
		T_MULT_ASSIGN,				//  *=
		T_DIV_ASSIGN,				//  /=
		T_MOD_ASSIGN,				//  %=

		T_BITNOT_ASSIGN,			//  ~=
		T_BITAND_ASSIGN,			//  &=
		T_BITOR_ASSIGN,				//  |=
		T_BITXOR_ASSIGN,			//  ^=
		T_BITSHIFT_LEFT_ASSIGN,		//	<<=
		T_BITSHIFT_RIGHT_ASSIGN,	//	>>=

		// Comparisson Operators:
		T_EQUALS,					//  ==
		T_NOT_EQUALS,				//  !=
		T_LESS_THAN,				//  <
		T_GREATER_THAN,				//  >
		T_LESS_EQUALS,				//  <=
		T_GREATER_EQUALS,			//  >=

		// Brackets:
		T_OPEN_PARENTHESIS,			// (
		T_CLOSE_PARENTHESIS,		// )

		T_OPEN_BRACKETS,			// [
		T_CLOSE_BRACKETS,			// ]

		T_OPEN_BRACES,				// {
		T_CLOSE_BRACES,				// }

		// Punctuation:
		T_QUESTION_MARK,			// ?
		T_COLON,					// :
		T_SEMICOLON,				// ;
		T_COMMA,					// ,
		T_DOT,						// .

		// Keywords:
		T_VAR,						// var
		T_FUNC,						// func

		T_IF,						// if
		T_ELSE,						// else

		T_FOR,						// for
		T_DO,						// do
		T_WHILE,					// while

		T_SWITCH,					// switch
		T_CASE,						// case
		T_CONTINUE,					// continue
		T_BREAK,					// break
		T_RETURN,					// return

		// Values:
		T_TRUE,						// true
		T_FALSE,					// false
		T_NULL,						// null
	};

	inline const LookupTable<std::string_view, ReservedToken> operatorTokenTable{
		{ "+", ReservedToken::T_ADD },
		{ "-", ReservedToken::T_SUB },
		{ "*", ReservedToken::T_MULT },
		{ "/", ReservedToken::T_DIV },
		{ "%", ReservedToken::T_MOD },

		{ "++", ReservedToken::T_INCREMENT },
		{ "--", ReservedToken::T_DECREMENT },

		{ "~", ReservedToken::T_BITNOT },
		{ "&", ReservedToken::T_BITAND },
		{ "|", ReservedToken::T_BITOR },
		{ "^", ReservedToken::T_BITXOR },
		{ "<<", ReservedToken::T_BITSHIFT_LEFT },
		{ ">>", ReservedToken::T_BITSHIFT_RIGHT },

		{ "!", ReservedToken::T_NOT },
		{ "&&", ReservedToken::T_AND },
		{ "||", ReservedToken::T_OR },

		{ "=", ReservedToken::T_ASSIGN },
		{ "+=", ReservedToken::T_ADD_ASSIGN },
		{ "-=", ReservedToken::T_SUB_ASSIGN },
		{ "*=", ReservedToken::T_MULT_ASSIGN },
		{ "/=", ReservedToken::T_DIV_ASSIGN },
		{ "%=", ReservedToken::T_MOD_ASSIGN },

		{ "~=", ReservedToken::T_BITNOT_ASSIGN },
		{ "&=", ReservedToken::T_BITAND_ASSIGN },
		{ "|=", ReservedToken::T_BITOR_ASSIGN },
		{ "^=", ReservedToken::T_BITXOR_ASSIGN },
		{ "<<=", ReservedToken::T_BITSHIFT_LEFT_ASSIGN },
		{ ">>=", ReservedToken::T_BITSHIFT_RIGHT_ASSIGN },

		{ "==", ReservedToken::T_EQUALS },
		{ "!=", ReservedToken::T_NOT_EQUALS },
		{ "<", ReservedToken::T_LESS_THAN },
		{ ">", ReservedToken::T_GREATER_THAN },
		{ "<=", ReservedToken::T_LESS_EQUALS },
		{ ">=", ReservedToken::T_GREATER_EQUALS },

		{ "(", ReservedToken::T_OPEN_PARENTHESIS },
		{ ")", ReservedToken::T_CLOSE_PARENTHESIS },

		{ "[", ReservedToken::T_OPEN_BRACKETS },
		{ "]", ReservedToken::T_CLOSE_BRACKETS },

		{ "{", ReservedToken::T_OPEN_BRACES },
		{ "}", ReservedToken::T_CLOSE_BRACES },

		{ "?", ReservedToken::T_QUESTION_MARK },
		{ ":", ReservedToken::T_COLON },
		{ ";", ReservedToken::T_SEMICOLON },
		{ ",", ReservedToken::T_COMMA },
		{ ".", ReservedToken::T_DOT }
	};

	inline const LookupTable<std::string_view, ReservedToken> keywordTokenTable{
		{ "var", ReservedToken::T_VAR },
		{ "func", ReservedToken::T_FUNC },
		{ "if", ReservedToken::T_IF },
		{ "else", ReservedToken::T_ELSE },
		{ "for", ReservedToken::T_FOR },
		{ "do", ReservedToken::T_DO },
		{ "while", ReservedToken::T_WHILE },
		{ "switch", ReservedToken::T_SWITCH },
		{ "case", ReservedToken::T_CASE },
		{ "continue", ReservedToken::T_CONTINUE },
		{ "break", ReservedToken::T_BREAK },
		{ "return", ReservedToken::T_RETURN },
		{ "true", ReservedToken::T_TRUE },
		{ "false", ReservedToken::T_FALSE },
		{ "null", ReservedToken::T_NULL }
	};

	inline const LookupTable<ReservedToken, std::string_view> tokenConversionTable = ([]() {
		std::vector<std::pair<ReservedToken, std::string_view>> container;
		container.reserve(operatorTokenTable.size() + keywordTokenTable.size());

		for (const auto& token : operatorTokenTable)
			container.emplace_back(token.second, token.first);

		for (const auto& token : keywordTokenTable)
			container.emplace_back(token.second, token.first);

		return LookupTable<ReservedToken, std::string_view>(std::move(container));
		})();

	inline std::ostream& operator<<(std::ostream& out, ReservedToken token)
	{
		/*out << tokenConversionTable.find(token)->second;*/
		return out;
	}
}
