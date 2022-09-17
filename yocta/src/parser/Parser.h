#pragma once
#include "InputStream.h"
#include "Precedence.h"
#include "Expression.h"
#include "Error.h"
#include "Rule.h"

#include "Lexer.h"

namespace yo
{
	class Parser
	{
	public:
		explicit Parser(InputStream& stream);

	public:
		std::vector<Expression> parse();

	private:
		void declaration();

		void variableDeclaration();

		void statement();

		void printStatement();

		void expressionStatement();

		void expression();

		void finishParser();

	private:
		void parsePrecedence(Precedence precedence);

		bool matchesToken(ReservedToken value);

	private:
		void handleGrouping();

		void handleVariable();

		void handleUnary();

		void handleBinary();

		void handleLiteral();

		void handleNumber();

		void handleString();

	private:
		void advance();

		void consumeToken(ReservedToken token, std::string errorMessage);

		void consumeIdentifier(std::string errorMessage);

	private:
		const std::unordered_map<ReservedToken, Rule> reservedRules = 
		{
			{ ReservedToken::T_NONE, { nullptr, nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_ADD, { nullptr, std::bind(&Parser::handleBinary, this), Precedence::P_TERM } },
			{ ReservedToken::T_SUB, { std::bind(&Parser::handleUnary, this), std::bind(&Parser::handleBinary, this), Precedence::P_TERM }, },
			{ ReservedToken::T_MULT, { nullptr, std::bind(&Parser::handleBinary, this), Precedence::P_FACTOR } },
			{ ReservedToken::T_DIV, { nullptr, std::bind(&Parser::handleBinary, this), Precedence::P_FACTOR } },
			{ ReservedToken::T_MOD, { nullptr, std::bind(&Parser::handleBinary, this), Precedence::P_TERM } },

			{ ReservedToken::T_INCREMENT, { std::bind(&Parser::handleUnary, this), nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_DECREMENT, { std::bind(&Parser::handleUnary, this), nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_BITNOT, { std::bind(&Parser::handleUnary, this), nullptr, Precedence::P_TERM } },
			{ ReservedToken::T_BITAND, { nullptr, std::bind(&Parser::handleBinary, this), Precedence::P_TERM } },
			{ ReservedToken::T_BITOR, { nullptr, std::bind(&Parser::handleBinary, this), Precedence::P_TERM } },
			{ ReservedToken::T_BITXOR, { nullptr, std::bind(&Parser::handleBinary, this), Precedence::P_TERM } },
			{ ReservedToken::T_BITSHIFT_LEFT, { nullptr, std::bind(&Parser::handleBinary, this), Precedence::P_TERM } },
			{ ReservedToken::T_BITSHIFT_RIGHT, { nullptr, std::bind(&Parser::handleBinary, this), Precedence::P_TERM } },

			{ ReservedToken::T_NOT, { std::bind(&Parser::handleUnary, this), nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_AND, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_OR, { nullptr, nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_ASSIGN, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_ADD_ASSIGN, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_SUB_ASSIGN, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_MULT_ASSIGN, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_DIV_ASSIGN, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_MOD_ASSIGN, { nullptr, nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_BITNOT_ASSIGN, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_BITAND_ASSIGN, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_BITOR_ASSIGN, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_BITXOR_ASSIGN, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_BITSHIFT_LEFT_ASSIGN, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_BITSHIFT_RIGHT_ASSIGN, { nullptr, nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_EQUALS, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_NOT_EQUALS, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_LESS_THAN, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_GREATER_THAN, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_LESS_EQUALS, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_GREATER_EQUALS, { nullptr, nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_OPEN_PARENTHESIS, { std::bind(&Parser::handleGrouping, this), nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_CLOSE_PARENTHESIS, { nullptr, nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_OPEN_BRACKETS, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_CLOSE_BRACKETS, { nullptr, nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_OPEN_BRACES, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_CLOSE_BRACES, { nullptr, nullptr, Precedence::P_NONE } },
			
			{ ReservedToken::T_QUESTION_MARK, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_COLON, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_SEMICOLON, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_COMMA, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_DOT, { nullptr, nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_VAR, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_FUNC, { nullptr, nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_IF, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_ELSE, { nullptr, nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_FOR, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_DO, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_WHILE, { nullptr, nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_SWITCH, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_CASE, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_CONTINUE, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_BREAK, { nullptr, nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_RETURN, { nullptr, nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_TRUE, { std::bind(&Parser::handleLiteral, this), nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_FALSE, { std::bind(&Parser::handleLiteral, this), nullptr, Precedence::P_NONE } },
			{ ReservedToken::T_NULL, { std::bind(&Parser::handleLiteral, this), nullptr, Precedence::P_NONE } },

			{ ReservedToken::T_PRINT, { nullptr, nullptr, Precedence::P_NONE } }
		};

	private:
		Rule getRule(YoctaValue value);

	private:
		Token m_CurrentToken, m_PreviousToken;
		std::vector<Expression> m_Results;

	private:
		InputStream& m_Stream;
	};
}