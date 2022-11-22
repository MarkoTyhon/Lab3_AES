#include "fileParser.h"

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

void FileParser::getCommands(std::string path) {

	std::ifstream inFile;
	std::string line;

	inFile.open(path);
	if (!inFile) {
		std::cout << "Unable to open file " << path << "!" << "\n";
		cmd_list.push_back("NONE");
	}
	else {
		std::cout << "Executing from '" << path << "'..." << "\n";
		while (std::getline(inFile, line))
		{
			cmd_list.push_back(line);
			std::cout << "Added: '" << line << "'\n";
		}
		std::cout << "Executing completed!" << "\n";
	}
	inFile.close();
}
