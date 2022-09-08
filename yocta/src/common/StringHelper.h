#pragma once

namespace yo
{
	inline bool endOfStream(const char* stream)
	{
		return *stream == '\0';
	}

	inline bool matches(const char* data, char expected)
	{
		if (endOfStream(data))
			return false;

		if (*data != expected)
			return false;

		data++;
		return true;
	}
}