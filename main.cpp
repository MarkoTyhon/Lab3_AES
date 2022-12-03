#include "fileParser.h"

#include <vector>
#include <iostream>
#include "IEEE754.h"
#include <string>
#include <regex>

std::string ltrim(const std::string& s) {
	return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
}

std::string rtrim(const std::string& s) {
	return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
}

std::string trim(const std::string& s) {
	return ltrim(rtrim(s));
}

std::pair<std::string, std::string> split(std::string str, int dlmtr_ln, size_t index) {
	std::pair<std::string, std::string> res;
	res.first = str.substr(0, (int)index);
	res.second = str.substr(index + dlmtr_ln, str.size() - (int)index + dlmtr_ln);
	return res;
}

int main() {
	/*double a = -1.2;
	double b = -0.000000000000000000000000001;
	IEEE754 i = IEEE754(a);
	IEEE754 j = IEEE754(b);
	IEEE754 res = IEEE754(a*b);
	std::cout << i << "\n" << j << "\n";
	
	std::cout << (i * j) << "\n";
	std::cout << res << "\n";*/


	std::string s = " \t\n  Hello World  ";

	s=  trim(s);

	std::size_t index;
	std::pair<std::string, std::string> operands;

	if ((index = s.find(" ")) != std::string::npos)
		operands = split(s, 1, index);
	std::cout << operands.first<< "\n" << operands.second;

	return 0;
}