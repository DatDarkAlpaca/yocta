#pragma once
#include "Expression.h"

namespace yo
{
	inline void disassembleParser(const std::vector<yo::Expression>& expressions)
	{
#ifdef DEBUG_LEXER_TRACE
		printf("\n");
#endif

		printf("-=-= Parser Expressions =-=-\n");

		for (const auto& expression : expressions)
		{
			auto exprValue = expression.value;

			printf("%s\t", translateExpression(expression.type));

			if (exprValue.isReservedToken())
			{
				auto a = tokenConversionTable.find(exprValue.getReservedToken());
				printf("%s\n", (*a).second.data());
			}

			else if (exprValue.isEOF())
				printf("\n");

			else
				printf("%s\n", translateYoctaValue(exprValue).c_str());
		}
	}
}