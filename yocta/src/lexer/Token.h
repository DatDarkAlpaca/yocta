#pragma once
#include "YoctaValue.h"

namespace yo
{
	class Token
	{
	public:
		Token() = default;

		Token(YoctaValue value, size_t lineNumber, size_t charIndex)
			: m_Value(std::move(value)), m_LineNumber(lineNumber), m_CharIndex(charIndex) { }

	public:
		size_t getLineNumber() const { return m_LineNumber; }

		size_t getCharIndex() const { return m_CharIndex; }

		YoctaValue getValue() const { return m_Value; }

	private:
		YoctaValue m_Value = {};
		size_t m_LineNumber = 0;
		size_t m_CharIndex = 0;
	};
}