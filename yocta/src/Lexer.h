#pragma once
#include <vector>
#include <string>

#include "Token.h"

namespace yo
{
	static std::vector<Token> lex(const std::string& input)
	{
		std::vector<Token> tokens;
		unsigned int currentPos = 0;

		while (currentPos < input.length())
		{
			unsigned int tokenStartPos = currentPos;
			char currentChar = input[currentPos];

			if (std::isspace(currentChar))
			{
				currentPos++;
				continue;
			}

			// Handles arithmetic operators:
			switch (currentChar)
			{
			case '+':
				currentPos++;
				tokens.push_back({ Token::Type::TY_PLUS_SIGN, std::string(1, currentChar), tokenStartPos });
				continue;
			case '-':
				currentPos++;
				tokens.push_back({ Token::Type::TY_MINUS_SIGN, std::string(1, currentChar), tokenStartPos });
				continue;
			case '*':
				currentPos++;
				tokens.push_back({ Token::Type::TY_MULT_SIGN, std::string(1, currentChar), tokenStartPos });
				continue;
			case '/':
				currentPos++;
				tokens.push_back({ Token::Type::TY_DIV_SIGN, std::string(1, currentChar), tokenStartPos });
				continue;
			case '(':
				currentPos++;
				tokens.push_back({ Token::Type::TY_LEFT_PARENTH, std::string(1, currentChar), tokenStartPos });
				continue;
			case ')':
				currentPos++;
				tokens.push_back({ Token::Type::TY_RIGHT_PARENTH, std::string(1, currentChar), tokenStartPos });
				continue;
			}

			// Handles digits:
			if (std::isdigit(currentChar) || currentChar == '.')
			{
				std::string text;
				unsigned int dotCount = 0;
				bool floatSignFound = false, doubleSignFound = false;

				std::string valid = "0123456789.fd";
				while (currentPos < input.length() && valid.find(input[currentPos]) != std::string::npos)
				{
					// Handles dots:
					if (input[currentPos] == '.')
					{
						if (dotCount == 1)
							break;

						dotCount++;
					}

					// Handles 'f' digits:
					else if (input[currentPos] == 'f')
					{
						floatSignFound = true;
						currentPos++;
						text += 'f';
						break;
					}

					else if (input[currentPos] == 'd')
					{
						doubleSignFound = true;
						currentPos++;
						break;
					}

					text += input[currentPos];
					currentPos++;
				}

				if (dotCount == 0)
					tokens.push_back({ Token::Type::TY_INT, text, tokenStartPos });
				else
				{
					if (floatSignFound)
						tokens.push_back({ Token::Type::TY_FLOAT, text, tokenStartPos });
					else if (doubleSignFound)
						tokens.push_back({ Token::Type::TY_DOUBLE, text, tokenStartPos });
					else
						tokens.push_back({ Token::Type::TY_DOUBLE, text, tokenStartPos });
				}

				continue;
			}

			// Handles text:
			else if (std::isalnum(currentChar))
			{
				std::string text;
				while (currentPos < input.length() && std::isalnum(input[currentPos]))
				{
					text += input[currentPos];
					currentPos++;
				}

				// Handles booleans and text:
				if (text == "true")
					tokens.push_back({ Token::Type::TY_TRUE, text, tokenStartPos });
				else if (text == "false")
					tokens.push_back({ Token::Type::TY_FALSE, text, tokenStartPos });
				else
					tokens.push_back({ Token::Type::TY_IDENTIFIER, text, tokenStartPos });

				continue;
			}

			else
				throw "An invalid character has been provided to the lexer.";
		}

		return tokens;
	}
}