#pragma once
#include <variant>
#include <memory>
#include "Value.h"
#include "YoctaObject.h"

namespace yo
{
	enum class ValueType
	{
		VT_NONE,
		VT_BOOL,
		VT_NUMERIC,
		VT_OBJECT
	};

	using YoctaValue = double;

	struct YoctaObject;
	struct StringObject;
	
	struct Value
	{
	public:
		Value() = default;

		Value(YoctaObject* object)
			: type(ValueType::VT_OBJECT), variantValue(object) { }

		Value(bool boolean)
			: type(ValueType::VT_BOOL), variantValue(boolean) { }

		Value(double number)
			: type(ValueType::VT_NUMERIC), variantValue(number) { }

		Value(const std::string& str)
			: type(ValueType::VT_OBJECT), variantValue((YoctaObject*)(new StringObject(str))) { }

	public:
		ValueType type = ValueType::VT_NONE;
		std::variant<bool, double, YoctaObject*> variantValue = false;

	public:
		friend Value operator-(const Value& lhs);

		friend Value operator+(const Value& lhs, const Value& rhs);

		friend Value operator-(const Value& lhs, const Value& rhs);

		friend Value operator*(const Value& lhs, const Value& rhs);

		friend Value operator/(const Value& lhs, const Value& rhs);

		friend const bool operator==(const Value& lhs, const Value& rhs);

		friend const bool operator<(const Value& lhs, const Value& rhs);

		friend const bool operator>(const Value& lhs, const Value& rhs);
	};

	inline StringObject* getStringObject(const Value& value)
	{
		return dynamic_cast<StringObject*>(std::get<YoctaObject*>(value.variantValue));
	}

	inline void displayValue(const Value& value)
	{
		if(value.type == ValueType::VT_NONE)
			printf("none");

		else if (value.type == ValueType::VT_BOOL)
			printf(std::get<bool>(value.variantValue) ? "true" : "false");

		else if (value.type == ValueType::VT_NUMERIC)
			printf("%f", std::get<double>(value.variantValue));

		else if (value.type == ValueType::VT_OBJECT)
		{
			std::string str = getStringObject(value)->data;

			switch (std::get<YoctaObject*>(value.variantValue)->type)
			{
				case ObjectType::STRING:
					printf("%s", str.c_str());
					break;
			}
		}
	}

	inline Value operator-(const Value& lhs)
	{
		double a = std::get<double>(lhs.variantValue);
		return { -a };
	}

	inline Value operator+(const Value& lhs, const Value& rhs)
	{
		if (lhs.type != rhs.type)
			return {};

		if (lhs.type == ValueType::VT_NONE || rhs.type == ValueType::VT_NONE)
			return {};

		if (lhs.variantValue.index() == 1)
		{
			double a = std::get<double>(lhs.variantValue);
			double b = std::get<double>(rhs.variantValue);

			return { a + b };
		}
		else if (lhs.variantValue.index() == 2)
		{
			std::string a = getStringObject(lhs)->data;
			std::string b = getStringObject(rhs)->data;

			std::string c = a.append(b);
			return { c };
		}

		return {};
	}

	inline Value operator-(const Value& lhs, const Value& rhs)
	{
		double a = std::get<double>(lhs.variantValue);
		double b = std::get<double>(rhs.variantValue);

		return { a - b };
	}

	inline Value operator*(const Value& lhs, const Value& rhs)
	{
		double a = std::get<double>(lhs.variantValue);
		double b = std::get<double>(rhs.variantValue);

		return { a * b };
	}

	inline Value operator/(const Value& lhs, const Value& rhs)
	{
		double a = std::get<double>(lhs.variantValue);
		double b = std::get<double>(rhs.variantValue);

		return { a / b };
	}

	inline const bool operator==(const Value& lhs, const Value& rhs)
	{
		if (lhs.type != rhs.type)
			return false;

		if (lhs.type == ValueType::VT_NONE)
			return (rhs.type == ValueType::VT_NONE);
			
		if (lhs.variantValue.index() == 0)
		{
			bool a = std::get<bool>(lhs.variantValue);
			bool b = std::get<bool>(rhs.variantValue);
			return a == b;
		}
		else if (lhs.variantValue.index() == 1)
		{
			double a = std::get<double>(lhs.variantValue);
			double b = std::get<double>(rhs.variantValue);
			return a == b;
		}

		return false;
	}

	inline const bool operator<(const Value& lhs, const Value& rhs)
	{
		if (lhs.type != rhs.type)
			return false;

		if (lhs.type == ValueType::VT_NONE)
			return true;

		if (lhs.variantValue.index() == 0)
		{
			bool a = std::get<bool>(lhs.variantValue);
			bool b = std::get<bool>(rhs.variantValue);
			return a < b;
		}
		else if (lhs.variantValue.index() == 1)
		{
			double a = std::get<double>(lhs.variantValue);
			double b = std::get<double>(rhs.variantValue);
			return a < b;
		}

		return false;
	}

	inline const bool operator>(const Value& lhs, const Value& rhs)
	{
		if (lhs.type != rhs.type)
			return false;

		if (lhs.type == ValueType::VT_NONE)
			return true;

		if (lhs.variantValue.index() == 0)
		{
			bool a = std::get<bool>(lhs.variantValue);
			bool b = std::get<bool>(rhs.variantValue);
			return a > b;
		}
		else if (lhs.variantValue.index() == 1)
		{
			double a = std::get<double>(lhs.variantValue);
			double b = std::get<double>(rhs.variantValue);
			return a > b;
		}

		return false;
	}
}