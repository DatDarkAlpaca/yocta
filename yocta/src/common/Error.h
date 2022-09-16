#pragma once
#include <exception>
#include <string>

namespace yo
{
	class Error : public std::exception
	{
	public:
		Error(std::string message, size_t lineNumber, size_t charIndex) noexcept
			: m_Message(std::move(message)), m_LineNumber(lineNumber), m_CharIndex(charIndex) { }

	public:
		const char* what() const throw () override
		{
			return m_Message.c_str();
		}

		size_t getLineNumber() const noexcept
		{
			return m_LineNumber;
		}

		size_t getCharIndex() const noexcept
		{
			return m_CharIndex;
		}

	private:
		std::string m_Message;
		size_t m_LineNumber;
		size_t m_CharIndex;
	};

	class LexingError : public Error
	{
	public:
		LexingError(std::string details, size_t lineNumber, size_t charIndex)
			: Error("Lexing Error: " + details, lineNumber, charIndex) { }
	};

	class ParserError : public Error
	{
	public:
		ParserError(std::string details, size_t lineNumber, size_t charIndex)
			: Error("Parser Error: " + details, lineNumber, charIndex) { }
	};
}