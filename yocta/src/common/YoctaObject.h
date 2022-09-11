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
	public:
		YoctaObject(ObjectType type)
			: type(type) { }

		virtual ~YoctaObject() = default;

	public:
		ObjectType type;
	};

	struct StringObject : public YoctaObject
	{
	public:
		StringObject(const std::string& string)
			: YoctaObject(ObjectType::STRING), data(string) {}

	public:
		std::string data;
	};
}