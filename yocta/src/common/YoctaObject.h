#pragma once
#include <string>

namespace yo
{
	enum class ObjectType
	{
		NONE = 0,
		STRING
	};

	struct YoctaObject
	{
		YoctaObject(ObjectType type)
			: type(type) { }

		ObjectType type;
	};

	struct StringObject
	{
	public:
		StringObject(const std::string& string)
			: object(ObjectType::STRING), data(string) {}

		YoctaObject object;
		std::string data;
	};
}