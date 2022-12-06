#include "fileParser.h"
#include "IEEE754.h"
#include "FPU.h"

#include <vector>
#include <iostream>
#include <string>


int main() {
	FileParser parser;
	parser.getCommands("cmd.txt");

	IEEE754 ieee;
	std::cout << "\nzero = " << ieee.getZero() << "\n";
	std::cout << "+inf = " << ieee.getInf(0) << "\n";
	std::cout << "-inf = " << ieee.getInf(1) << "\n";
	std::cout << "NaN = " << ieee.getNaN() << "\n";
	std::cout << "Maximal normalized number = " << ieee.getMaxNormal() << "\n";
	std::cout << "Minimal normalized number = " << ieee.getMinNormal() << "\n";
	std::cout << "Maximal denormalized number = " << ieee.getMaxDenormal() << "\n";
	std::cout << "Minimal denormalized number = " << ieee.getMinDenormal() << "\n";
	std::cout << "+1,0E0 = " << ieee.get10E0() << "\n";
	std::cout << "F(a, b) = 2*a*b / (a*a + b*b)" << "\n";

	std::cout << "\n";


	FPU fpu;
	fpu.mainProc(parser.cmd_list);


	return 0;
}