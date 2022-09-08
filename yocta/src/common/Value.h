#pragma once
#include <variant>

namespace yo
{
	enum class ValueType
	{
		VT_NONE,
		VT_BOOL,
		VT_NUMERIC
	};

	using yocta_value = double;

	struct Value
	{
	public:
		ValueType type;
		std::variant<bool, double> variantValue;

	public:
		friend const Value& operator-(const Value& lhs);

		friend const Value& operator+(const Value& lhs, const Value& rhs);

		friend const Value& operator-(const Value& lhs, const Value& rhs);

		friend const Value& operator*(const Value& lhs, const Value& rhs);

		friend const Value& operator/(const Value& lhs, const Value& rhs);
	};

	inline void displayValue(const Value& value)
	{
		if(value.type == ValueType::VT_NONE)
			printf("none");

		else if (value.type == ValueType::VT_BOOL)
			printf(std::get<bool>(value.variantValue) ? "true" : "false");

		else if (value.type == ValueType::VT_NUMERIC)
			printf("%f", std::get<double>(value.variantValue));
	}

	inline const Value& operator-(const Value& lhs)
	{
		double a = std::get<double>(lhs.variantValue);
		return { lhs.type, std::variant<bool, double>(-a) };
	}

	inline const Value& operator+(const Value& lhs, const Value& rhs)
	{
		double a = std::get<double>(lhs.variantValue);
		double b = std::get<double>(lhs.variantValue);

		return { lhs.type, std::variant<bool, double>(a + b) };
	}

	inline const Value& operator-(const Value& lhs, const Value& rhs)
	{
		double a = std::get<double>(lhs.variantValue);
		double b = std::get<double>(lhs.variantValue);

		return { lhs.type, std::variant<bool, double>(a - b) };
	}

	inline const Value& operator*(const Value& lhs, const Value& rhs)
	{
		double a = std::get<double>(lhs.variantValue);
		double b = std::get<double>(lhs.variantValue);

		return { lhs.type, std::variant<bool, double>(a * b) };
	}

	inline const Value& operator/(const Value& lhs, const Value& rhs)
	{
		double a = std::get<double>(lhs.variantValue);
		double b = std::get<double>(lhs.variantValue);

		return { lhs.type, std::variant<bool, double>(a / b) };
	}
}