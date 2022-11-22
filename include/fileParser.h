#pragma once

#include <fstream>
#include <vector>
#include <string>

struct FileParser
{
	void getCommands(std::string path);
	std::vector <std::string> cmd_list;
};