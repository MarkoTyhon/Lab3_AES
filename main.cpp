#include "fileParser.h"

#include <iostream>
#include "Stack.h"


int main() {
	Stack<int> s;



	FileParser parser;
	parser.getCommands("Text.txt");

	for (int i = 0; i < std::size(parser.cmd_list); i++) {
		std::cout << parser.cmd_list[i];
	}

	return 0;
}