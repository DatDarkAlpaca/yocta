#include <iostream>
#include <string>
#include "Lexer.h"

int main()
{
	std::string input;
	std::getline(std::cin, input);

	auto tokens = yo::lex(input);
	std::cout << "Tokens: " << tokens.size() << '\n';

	for (const auto& token : tokens)
	{
		std::cout << "- Text: " << token.text << " | ";
		std::cout << "- Type: " << (int)token.type << " | ";
		std::cout << "- Start: " << token.startPosition << "\n";
	}
}