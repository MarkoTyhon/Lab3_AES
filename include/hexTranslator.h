#pragma once

#include <string>
#include <vector>
#include "IEEE754.h"
#include <map>


struct HexTranslator {
	void numToHex(IEEE754 bin_num);

	std::map<std::string, std::string> hex_dic = {
		{ "0000", "0"},
		{ "0001", "1"},
		{ "0010", "2"},
		{ "0011", "3"},
		{ "0100", "4"},
		{ "0101", "5"},
		{ "0110", "6"},
		{ "0111", "7"},
		{ "1000", "8"},
		{ "1001", "9"},
		{ "1010", "A"},
		{ "1011", "B"},
		{ "1100", "C"},
		{ "1101", "D"},
		{ "1110", "E"},
		{ "1111", "F"}
	};


	std::string bin_num = "";
	std::string str_hex_num = "";
};