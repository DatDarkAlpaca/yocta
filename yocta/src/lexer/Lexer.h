#pragma once
#include "InputStream.h"
#include "Token.h"

namespace yo
{
	Token lex(InputStream& stream);
}