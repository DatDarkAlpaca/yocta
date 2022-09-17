#pragma once
#include <string>
#include <sstream>
#include <variant>

#include "ReservedToken.h"

namespace yo
{
	struct Identifier
	{
	public:
		Identifier(const std::string& name) : name(name) { }

	public:
		std::string name;
	};

	struct END_OF_FILE { };

	struct NULL_TYPE { };

	struct YoctaValue
	{
	private:
		using value_type = std::variant<ReservedToken, Identifier, std::string, double, bool, NULL_TYPE, END_OF_FILE>;

	public:
		bool isReservedToken() const { return std::holds_alternative<ReservedToken>(value); }

		bool isIdentifier() const { return std::holds_alternative<Identifier>(value); }

		bool isString() const { return std::holds_alternative<std::string>(value); }

		bool isNumber() const { return std::holds_alternative<double>(value); }

		bool isBool() const { return std::holds_alternative<bool>(value); }

		bool isEOF() const { return std::holds_alternative<END_OF_FILE>(value); }

		bool isNull() const { return std::holds_alternative<NULL_TYPE>(value); }

	public:
		ReservedToken getReservedToken() const { return std::get<ReservedToken>(value); }

		Identifier getIdentifier() const { return std::get<Identifier>(value); }

		std::string getString() const { return std::get<std::string>(value); }

		double getNumber() const { return std::get<double>(value); }

		double getBool() const { return std::get<bool>(value); }

		NULL_TYPE getNull() const { return std::get<NULL_TYPE>(value); }

		value_type getValue() const { return value; }

	public:
		YoctaValue operator+(const YoctaValue& other) const
		{
			if (this->value.index() != other.value.index())
				return YoctaValue();

			if (this->isNumber())
				return { this->getNumber() + other.getNumber() };

			else if (this->isString())
				return { this->getString() + other.getString() };

			else
				return YoctaValue();
		}

		YoctaValue operator-(const YoctaValue& other) const
		{
			if (this->value.index() != other.value.index())
				return YoctaValue();

			if (this->isNumber())
				return { this->getNumber() - other.getNumber() };

			else
				return YoctaValue();
		}

		YoctaValue operator*(const YoctaValue& other) const
		{
			if (this->value.index() != other.value.index())
				return YoctaValue();

			if (this->isNumber())
				return { this->getNumber() * other.getNumber() };

			else
				return YoctaValue();
		}

		YoctaValue operator/(const YoctaValue& other) const
		{
			if (this->value.index() != other.value.index())
				return YoctaValue();

			if (this->isNumber())
				return { this->getNumber() / other.getNumber() };

			else
				return YoctaValue();
		}

		YoctaValue operator%(const YoctaValue& other) const
		{
			if (this->value.index() != other.value.index())
				return YoctaValue();

			if (this->isNumber())
				return { double((int)this->getNumber() % (int)other.getNumber()) };

			else
				return YoctaValue();
		}

		friend bool operator==(const YoctaValue& lhs, const YoctaValue& rhs);

		friend bool operator!=(const YoctaValue& lhs, const YoctaValue& rhs);

	public:
		value_type value = END_OF_FILE();
	};

	inline bool operator==(const YoctaValue& lhs, const YoctaValue& rhs)
	{
		if (lhs.value.index() != rhs.value.index())
			return false;

		if (lhs.isReservedToken())
			return lhs.getReservedToken() == rhs.getReservedToken();

		else if (lhs.isIdentifier())
			return lhs.getIdentifier().name == rhs.getIdentifier().name;

		else if (lhs.isString())
			return lhs.getString() == rhs.getString();

		else if (lhs.isNumber())
			return lhs.getNumber() == rhs.getNumber();

		else if (lhs.isBool())
			return lhs.getBool() == rhs.getBool();

		else if (lhs.isNull())
			return false;

		return false;
	}

	inline bool operator!=(const YoctaValue& lhs, const YoctaValue& rhs)
	{
		if (lhs.value.index() != rhs.value.index())
			return true;

		if (lhs.isReservedToken())
			return lhs.getReservedToken() != rhs.getReservedToken();

		else if (lhs.isIdentifier())
			return lhs.getIdentifier().name != rhs.getIdentifier().name;

		else if (lhs.isString())
			return lhs.getString() != rhs.getString();

		else if (lhs.isNumber())
			return lhs.getNumber() != rhs.getNumber();

		else if (lhs.isBool())
			return lhs.getBool() != rhs.getBool();

		else if (lhs.isNull())
			return true;

		return true;
	}

	inline std::string translateYoctaValue(const YoctaValue& value)
	{
		if (value.isReservedToken())
		{
			auto tokenName = tokenConversionTable.find(value.getReservedToken());
			return tokenName->second.data();
		}

		else if (value.isIdentifier())
			return value.getIdentifier().name;

		else if (value.isString())
			return value.getString();

		else if (value.isNumber())
		{
			std::ostringstream oss;
			oss << value.getNumber();
			return oss.str();
		}

		else if (value.isBool())
			return value.getBool() ? "true" : "false";

		else if (value.isNull())
			return "null";

		return "Unknown YoctaValue";
	}
}