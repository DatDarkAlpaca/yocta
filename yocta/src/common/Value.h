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
		ValueType type;
		std::variant<bool, double, YoctaObject*> variantValue;

	public:
		friend const Value& operator-(const Value& lhs);

		friend const Value& operator+(const Value& lhs, const Value& rhs);

		friend const Value& operator-(const Value& lhs, const Value& rhs);

		friend const Value& operator*(const Value& lhs, const Value& rhs);

		friend const Value& operator/(const Value& lhs, const Value& rhs);

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
			auto str = getStringObject(value)->data;

			switch (std::get<YoctaObject*>(value.variantValue)->type)
			{
				case ObjectType::STRING:
					printf("%s", str.c_str());
					break;
			}
		}
	}

	inline const Value& operator-(const Value& lhs)
	{
		double a = std::get<double>(lhs.variantValue);
		std::variant<bool, double, YoctaObject*> v(-a);
		return { lhs.type, v };
	}

	inline const Value& operator+(const Value& lhs, const Value& rhs)
	{
		if (lhs.type != rhs.type)
			return {};

		if (lhs.variantValue.index() == 1)
		{
			double a = std::get<double>(lhs.variantValue);
			double b = std::get<double>(rhs.variantValue);

			std::variant<bool, double, YoctaObject*>v(a + b);
			return { lhs.type, v };
		}
		else if (lhs.variantValue.index() == 2)
		{
			std::string a = getStringObject(lhs)->data;
			std::string b = getStringObject(rhs)->data;

			std::string c = a.append(b);

			std::variant<bool, double, YoctaObject*>v((YoctaObject*)(new StringObject(c)));
			return { lhs.type, v };
		}
	}

	inline const Value& operator-(const Value& lhs, const Value& rhs)
	{
		double a = std::get<double>(lhs.variantValue);
		double b = std::get<double>(rhs.variantValue);

		std::variant<bool, double, YoctaObject*> v(a - b);
		return { lhs.type, v };
	}

	inline const Value& operator*(const Value& lhs, const Value& rhs)
	{
		double a = std::get<double>(lhs.variantValue);
		double b = std::get<double>(rhs.variantValue);

		std::variant<bool, double, YoctaObject*> v(a * b);
		return { lhs.type, v };
	}

	inline const Value& operator/(const Value& lhs, const Value& rhs)
	{
		double a = std::get<double>(lhs.variantValue);
		double b = std::get<double>(rhs.variantValue);

		std::variant<bool, double, YoctaObject*> v(a / b);
		return { lhs.type, v };
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
	}
}