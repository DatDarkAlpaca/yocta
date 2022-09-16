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
		void expression();

	private:
		void parsePrecedence(Precedence precedence);

		bool matchesToken(YoctaValue value);

	private:
		void handleGrouping();

		void handleUnary();

		void handleBinary();

		void handleLiteral();

		void handleNumber();

		void handleString();

	private:
		void advance();

		void consumeToken(YoctaValue value, std::string errorMessage);

	private:
		const std::unordered_map<ReservedToken, Rule> reservedRules = 
		{
			{ ReservedToken::T_ADD, { nullptr, std::bind(&Parser::handleBinary, this), Precedence::P_TERM }, },
			{ ReservedToken::T_SUB, { std::bind(&Parser::handleUnary, this), std::bind(&Parser::handleBinary, this), Precedence::P_TERM }, },
			{ ReservedToken::T_MULT, { nullptr, std::bind(&Parser::handleBinary, this), Precedence::P_FACTOR }, },
			{ ReservedToken::T_DIV, { nullptr, std::bind(&Parser::handleBinary, this), Precedence::P_FACTOR }, },
			{ ReservedToken::T_MOD, { nullptr, std::bind(&Parser::handleBinary, this), Precedence::P_FACTOR }, },

			{ ReservedToken::T_OPEN_PARENTHESIS, { std::bind(&Parser::handleGrouping, this), nullptr, Precedence::P_NONE }, },
			{ ReservedToken::T_CLOSE_PARENTHESIS, { nullptr, nullptr, Precedence::P_NONE }, },
			
			{ ReservedToken::T_SEMICOLON, { nullptr, nullptr, Precedence::P_NONE }, },

			{ ReservedToken::T_TRUE, { std::bind(&Parser::handleLiteral, this), nullptr, Precedence::P_NONE }, },
			{ ReservedToken::T_FALSE, { std::bind(&Parser::handleLiteral, this), nullptr, Precedence::P_NONE }, },
			{ ReservedToken::T_NULL, { std::bind(&Parser::handleLiteral, this), nullptr, Precedence::P_NONE }, }
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