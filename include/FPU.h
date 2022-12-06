#pragma once
#include "IEEE754.h"
#include <stack>
#include "Memory.h"

#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <stack>


#ifdef _WIN32
#include <conio.h>
#endif
#ifdef linux
#include <curses.h>
#endif

struct FPU
{	
	void mainProc(std::vector<std::string> cmdlist);

	void getCommand(std::string cmd);	
	void doCommand();
	void tact1();
	void tact2();
	IEEE754 getValue(int i);
	IEEE754 gtNVlFrStck(int indx);
	std::string ltrim(const std::string& s);
	std::string rtrim(const std::string& s);
	std::string trim(const std::string& s);
	std::pair<std::string, std::string> split(std::string str, int dlmtr_ln, size_t index);


	void opMOV();
	void opLOAD();
	void opCOPY();
	void opREVERSE();
	void opADD();
	void opSUBTRACT();
	void opMULT();
	void opDIVIS();
	void opGET();
	void showProc();
	int pressAnyKey();
	void gtAllVlFrStck();
	void toMemory();
	void deleteLastElem();

	std::string cmd[2];
	int TC = 0;
	int PS = 0;
	int PC = 0;
	std::string IR;

	std::stack<IEEE754> reg_stack;
	IEEE754 ieee;
	Memory mem;
};