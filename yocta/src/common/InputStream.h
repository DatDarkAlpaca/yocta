#pragma once
#include <functional>
#include <sstream>
#include <stack>

namespace yo
{
	class InputStream
	{
	public:
		InputStream(const std::string_view& inputFunction)
			: m_Stream(inputFunction) { }

	public:
		int popCharacter()
		{
			int result = -1;

			if (m_InputStack.empty())
			{
				// Resets the stream position count:
				if (m_Position >= m_Stream.size())
				{
					m_Position = 0;
					return result;
				}

				result = m_Stream.at(m_Position);
				m_Position++;
			}
			else
			{
				result = m_InputStack.top();
				m_InputStack.pop();
			}

			if (result == '\n')
				++m_LineNumber;

			++m_CharIndex;

			return result;
		}

		void pushCharacter(int character)
		{
			m_InputStack.push(character);

			if (character == '\n')
				--m_LineNumber;

			--m_CharIndex;
		}

		int peekCharacter() const
		{
			return m_InputStack.top();
		}

	public:
		size_t getLineNumber() const { return m_LineNumber; }

		size_t getCharIndex() const { return m_CharIndex; }

	private:
		const std::string_view& m_Stream;
		size_t m_Position = 0;

		std::stack<int> m_InputStack;
		size_t m_LineNumber = 0;
		size_t m_CharIndex = 0;
	};
}