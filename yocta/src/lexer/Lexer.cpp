#include <optional>
#include <string>

#include "ReservedToken.h"
#include "Lexer.h"
#include "Error.h"

namespace
{
	enum class CharacterType { END_OF_FILE = 0, WHITESPACE, ALPHA, NUMERIC, PUNCTUATION, INVALID = -1 };

	CharacterType getCharacterType(int character)
	{
		if (character == EOF)
			return CharacterType::END_OF_FILE;

		else if (std::isspace(character))
			return CharacterType::WHITESPACE;

		else if (character == '_' || std::isalpha(character))
			return CharacterType::ALPHA;

		else if (std::isdigit(character))
			return CharacterType::NUMERIC;

		else if (std::ispunct(character))
			return CharacterType::PUNCTUATION;

		return CharacterType::INVALID;
	}
}

namespace 
{
	std::optional<yo::ReservedToken> fetchKeyword(std::string_view word)
	{
		auto it = yo::keywordTokenTable.find(word);
		return it == yo::keywordTokenTable.end() ? std::nullopt : std::make_optional(it->second);
	}

	std::optional<yo::ReservedToken> fetchToken(std::string_view word)
	{
		auto it = yo::operatorTokenTable.find(word);
		return it == yo::operatorTokenTable.end() ? std::nullopt : std::make_optional(it->second);
	}
}

namespace 
{
	yo::Token handleIdentifier(yo::InputStream& stream)
	{
		using namespace yo;
		size_t lineNumber = stream.getLineNumber();
		size_t charIndex = stream.getCharIndex();
		std::string identifier;

		int character = stream.popCharacter();
		do
		{
			identifier += (char)character;
			character = stream.popCharacter();
		} while (std::isalnum(character) || character == '_');

		stream.pushCharacter(character);

		if (auto token = fetchKeyword(identifier))
			return Token({ token.value() }, lineNumber, charIndex);
		else
			return Token({ Identifier(identifier) }, lineNumber, charIndex);
	}

	yo::Token handleNumber(yo::InputStream& stream)
	{
		using namespace yo;

		size_t lineNumber = stream.getLineNumber();
		size_t charIndex = stream.getCharIndex();
		std::string number;

		int character = stream.popCharacter();
		do
		{
			number += (char)character;
			character = stream.popCharacter();
		} while (std::isdigit(character) || character == '.');

		stream.pushCharacter(character);

		double numberValue = std::atof(number.c_str());

		return Token({ numberValue }, lineNumber, charIndex);
	}

	yo::Token handleString(yo::InputStream& stream)
	{
		using namespace yo;

		size_t lineNumber = stream.getLineNumber();
		size_t charIndex = stream.getCharIndex();
		std::string string;

		int character = stream.popCharacter();
		do
		{
			string += (char)character;

			if (character == '\n')
				lineNumber++;

			character = stream.popCharacter();

		} while (character != '"' && getCharacterType(character) != CharacterType::END_OF_FILE);

		if (character != '"')
		{
			stream.pushCharacter(character);
			throw new LexingError("Missing close quote", stream.getLineNumber(), stream.getCharIndex());
		}

		return Token({ string }, lineNumber, charIndex);
	}

	yo::Token handleOperator(yo::InputStream& stream)
	{
		using namespace yo;

		size_t lineNumber = stream.getLineNumber();
		size_t charIndex = stream.getCharIndex();

		ReservedToken reserved = ReservedToken::T_NONE;
		switch (stream.popCharacter())
		{
		case '+':
		{
			int next = stream.popCharacter();

			if (next == '+')
				reserved = ReservedToken::T_INCREMENT;
			else if (next == '=')
				reserved = ReservedToken::T_ADD_ASSIGN;
			else
			{
				reserved = ReservedToken::T_ADD;
				stream.pushCharacter(next);
			}

			break;
		}

		case '-':
		{
			int next = stream.popCharacter();

			if (next == '-')
				reserved = ReservedToken::T_DECREMENT;
			else if (next == '=')
				reserved = ReservedToken::T_SUB_ASSIGN;
			else
			{
				reserved = ReservedToken::T_SUB;
				stream.pushCharacter(next);
			}

			break;
		}

		case '*':
		{
			int next = stream.popCharacter();

			if (next == '=')
				reserved = ReservedToken::T_MULT_ASSIGN;
			else
			{
				reserved = ReservedToken::T_MULT;
				stream.pushCharacter(next);
			}

			break;
		}

		case '/':
		{
			int next = stream.popCharacter();

			if (next == '=')
				reserved = ReservedToken::T_DIV_ASSIGN;
			else
			{
				reserved = ReservedToken::T_DIV;
				stream.pushCharacter(next);
			}

			break;
		}

		case '%':
		{
			int next = stream.popCharacter();

			if (next == '=')
				reserved = ReservedToken::T_MOD_ASSIGN;
			else
			{
				reserved = ReservedToken::T_MOD;
				stream.pushCharacter(next);
			}

			break;
		}

		case '~':
		{
			int next = stream.popCharacter();

			if (next == '=')
				reserved = ReservedToken::T_BITNOT_ASSIGN;
			else
			{
				reserved = ReservedToken::T_BITNOT;
				stream.pushCharacter(next);
			}

			break;
		}

		case '!':
		{
			int next = stream.popCharacter();

			if (next == '=')
				reserved = ReservedToken::T_NOT_EQUALS;
			else
			{
				stream.pushCharacter(next);
				reserved = ReservedToken::T_NOT;
			}
			break;
		}

		case '&':
		{
			int next = stream.popCharacter();

			if (next == '=')
				reserved = ReservedToken::T_BITAND_ASSIGN;
			else if (next == '&')
				reserved = ReservedToken::T_AND;
			else
			{
				reserved = ReservedToken::T_BITAND;
				stream.pushCharacter(next);
			}

			break;
		}

		case '|':
		{
			int next = stream.popCharacter();

			if (next == '=')
				reserved = ReservedToken::T_BITOR_ASSIGN;
			if (next == '|')
				reserved = ReservedToken::T_OR;
			else
			{
				reserved = ReservedToken::T_BITOR;
				stream.pushCharacter(next);
			}

			break;
		}

		case '^':
		{
			int next = stream.popCharacter();

			if (next == '=')
				reserved = ReservedToken::T_BITXOR_ASSIGN;
			else
			{
				reserved = ReservedToken::T_BITXOR;
				stream.pushCharacter(next);
			}

			break;
		}

		case '<':
		{
			int next = stream.popCharacter();

			if (next == '=')
				reserved = ReservedToken::T_LESS_EQUALS;
			else if (next == '<')
			{
				int attach = stream.popCharacter();

				if (attach == '=')
					reserved = ReservedToken::T_BITSHIFT_LEFT_ASSIGN;
				else
				{
					reserved = ReservedToken::T_BITSHIFT_LEFT;
					stream.pushCharacter(attach);
				}
			}
			else
			{
				reserved = ReservedToken::T_LESS_THAN;
				stream.pushCharacter(next);
			}

			break;
		}

		case '>':
		{
			int next = stream.popCharacter();

			if (next == '=')
				reserved = ReservedToken::T_GREATER_EQUALS;
			else if (next == '>')
			{
				int attach = stream.popCharacter();

				if (attach == '=')
					reserved = ReservedToken::T_BITSHIFT_RIGHT_ASSIGN;
				else
				{
					reserved = ReservedToken::T_BITSHIFT_RIGHT;
					stream.pushCharacter(attach);
				}
			}
			else
			{
				reserved = ReservedToken::T_GREATER_THAN;
				stream.pushCharacter(next);
			}

			break;
		}

		case '=':
		{
			int next = stream.popCharacter();

			if (next == '=')
				reserved = ReservedToken::T_EQUALS;
			else
			{
				stream.pushCharacter(next);
				reserved = ReservedToken::T_ASSIGN;
			}
			break;
		}

		case '(':
			reserved = ReservedToken::T_OPEN_PARENTHESIS;
			break;
		case ')':
			reserved = ReservedToken::T_CLOSE_PARENTHESIS;
			break;
		case '[':
			reserved = ReservedToken::T_OPEN_BRACKETS;
			break;
		case ']':
			reserved = ReservedToken::T_CLOSE_BRACKETS;
			break;
		case '{':
			reserved = ReservedToken::T_OPEN_BRACES;
			break;
		case '}':
			reserved = ReservedToken::T_CLOSE_BRACES;
			break;
		case '?':
			reserved = ReservedToken::T_QUESTION_MARK;
			break;
		case ':':
			reserved = ReservedToken::T_COLON;
			break;
		case ';':
			reserved = ReservedToken::T_SEMICOLON;
			break;
		case ',':
			reserved = ReservedToken::T_COMMA;
			break;
		case '.':
			reserved = ReservedToken::T_DOT;
			break;

		default:
			reserved = ReservedToken::T_NONE;
			break;
		}

		if (reserved != ReservedToken::T_NONE)
			return Token({ reserved }, lineNumber, charIndex);

		throw new LexingError("Unexpected token", lineNumber, charIndex);
		return {};
	}

	void handleInlineComment(yo::InputStream& stream)
	{
		auto isEOFCharacter = [](char character) -> bool {
			return character == '\n' || getCharacterType(character) == CharacterType::END_OF_FILE;
		};

		int character;
		do
		{
			character = stream.popCharacter();
		} while (!isEOFCharacter(character));

		if (character != '\n')
			stream.pushCharacter(character);
	}

	void handleBlockComment(yo::InputStream& stream)
	{
		using namespace yo;

		auto isEOFCharacter = [](char character) -> bool {
			return getCharacterType(character) == CharacterType::END_OF_FILE;
		};

		bool close = false;
		int character;

		do
		{
			character = stream.popCharacter();
			if (close && character == '/')
				return;

			close = (character == '*');

		} while (!isEOFCharacter(character));

		stream.pushCharacter(character);
		throw LexingError("Missing closing quote", stream.getLineNumber(), stream.getCharIndex());
	}
}

yo::Token yo::lex(yo::InputStream& stream)
{
	while (true)
	{
		int character = stream.popCharacter();

		switch (getCharacterType(character))
		{
			case CharacterType::END_OF_FILE:
			{
				size_t lineNumber = stream.getLineNumber();
				size_t charIndex = stream.getCharIndex();
				return { { yo::END_OF_FILE() }, lineNumber, charIndex };
			}

			case CharacterType::WHITESPACE:
				continue;

			case CharacterType::ALPHA:
				stream.pushCharacter(character);
				return handleIdentifier(stream);

			case CharacterType::NUMERIC:
				stream.pushCharacter(character);
				return handleNumber(stream);

			case CharacterType::PUNCTUATION:
			{
				switch (character)
				{
				case '"':
					return handleString(stream);

				case '/':
				{
					char nextCharacter = stream.popCharacter();
					switch (nextCharacter)
					{
						case '/':
							handleInlineComment(stream);
							continue;

						case '*':
							handleBlockComment(stream);
							continue;

						default:
							stream.pushCharacter(nextCharacter);
					}
				}

				default:
					stream.pushCharacter(character);
					return handleOperator(stream);
				}
				break;
			}
		}
	}
}
