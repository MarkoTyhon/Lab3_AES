#include "FPU.h"

void FPU::mainProc(std::vector<std::string> cmdlist) {
	int tact;
	int exit = 0;
	for (std::string curr_cmd : cmdlist) {
		getCommand(curr_cmd);
		tact = 0;
		if (exit == 27) break;
		while ((tact < 2) && (exit != 27)) {
			tact += 1;
			if (tact % 2)
				tact1();
			else
				tact2();
			exit = pressAnyKey();
		}
	}
}

void FPU::tact1() {
	TC = 1;
	IR = cmd[0];
	if (cmd[1] != "NONE")
		IR += " " + cmd[0];
	++PC;

	PS = getValue(1).sign;

	showProc();
}

void FPU::tact2() {
	TC = 2;
	doCommand();
	PS = getValue(1).sign;
	showProc();
}


IEEE754 FPU::gtNVlFrStck(int indx) {
	Stack<IEEE754> loc_stack;
	loc_stack = reg_stack;
	while (loc_stack.size() != indx + 1) {
		loc_stack.pop();
	}
	return loc_stack.top();
}

IEEE754 FPU::getValue(int indx) {
	if (cmd[indx].find("stack") != std::string::npos) {
		if (cmd[indx].find("[") == std::string::npos) {
			return reg_stack.top();
		}
		else {
			return gtNVlFrStck(std::stoi(cmd[indx].substr(cmd[indx].length() - 2, 1).c_str()));
		}
	}
	else if (cmd[indx].find("mem") != std::string::npos) {
		return mem.getFromMem(std::stoi(cmd[indx].substr(cmd[indx].length() - 2, 1).c_str()));
	}
	else {
		double dbl_value = std::stod(cmd[indx].c_str());
		return IEEE754(dbl_value);
	}
}

void FPU::toMemory() {
	mem.addToMem(gtNVlFrStck(7));
}

void FPU::opMOV() {
	if (reg_stack.full()) {
		toMemory();
	}
	reg_stack.push(getValue(1));
}

void FPU::opLOAD() {
	IEEE754 val = reg_stack.top();

	if (cmd[1].find("[") == std::string::npos)
		mem.addToMem(val);
	else
		mem.addToMem(std::stoi(cmd[1].substr(cmd[1].length() - 2, 1).c_str()), val);
}


void FPU::opCOPY() {
	reg_stack.copy();	
}

void FPU::opREVERSE() {
	reg_stack.reverse();
}

void FPU::opGET() {
	reg_stack.push(getValue(1));
}


void FPU::doCommand() {
	if (cmd[0] == "mov") {
		opMOV();
	}
	else if (cmd[0] == "load") {
		opLOAD();
	}
	else if (cmd[0] == "add") {
		opADD();
	}
	else if (cmd[0] == "sub") {
		opSUBTRACT();
	}
	else if (cmd[0] == "mult") {
		opMULT();
	}
	else if (cmd[0] == "div") {
		opDIVIS();
	}
	else if (cmd[0] == "copy") {
		opCOPY();
	}
	else if (cmd[0] == "reverse") {
		opREVERSE();
	}
	else if (cmd[0] == "get") {
		opGET();
	}
}

std::string FPU::ltrim(const std::string& s) {
	return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
}

std::string FPU::rtrim(const std::string& s) {
	return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
}

std::string FPU::trim(const std::string& s) {
	return ltrim(rtrim(s));
}

std::pair<std::string, std::string> FPU::split(std::string str, int dlmtr_ln, size_t index) {
	std::pair<std::string, std::string> res;
	res.first = str.substr(0, (int)index);
	res.second = str.substr(index + dlmtr_ln, str.size() - (int)index + dlmtr_ln);
	return res;
}

void FPU::getCommand(std::string command) {
	std::size_t index;
	std::pair<std::string, std::string> operands;
	command = trim(command);

	if ((index = command.find(" ")) != std::string::npos) {
		operands = split(command, 1, index);
		cmd[0] = operands.first;
		cmd[1] = operands.second;
	}
	else {
		cmd[0] = command;
		cmd[1] = "NONE";
	}
	
}

void FPU::opADD() {
	IEEE754 x = reg_stack.pop();
	IEEE754 y = reg_stack.pop();
	reg_stack.push(x + y);
}

void FPU::opSUBTRACT() {
	IEEE754 x = reg_stack.pop();
	IEEE754 y = reg_stack.pop();
	reg_stack.push(x - y);
}

void FPU::opMULT() {
	IEEE754 x = reg_stack.pop();
	IEEE754 y = reg_stack.pop();
	reg_stack.push(x * y);
}

void FPU::opDIVIS() {
	IEEE754 x = reg_stack.pop();
	IEEE754 y = reg_stack.pop();
	reg_stack.push(x / y);
}

int FPU::pressAnyKey() {
	std::cout << "\nPress any key to continue or ESC to exit...\n";
#if _WIN32
	int exit = _getch();
#endif
#if linux
	int exit = getch();
#endif
	return exit;
}

void FPU::gtAllVlFrStck() {
	Stack<IEEE754> loc_stack;
	loc_stack = reg_stack;
	IEEE754 buf;
	int ccount = -1;
	while (!loc_stack.empty()) {
		buf = loc_stack.top();
		for (int i = 0; i < BIT / 8; i++) {
			std::cout << loc_stack.pop() << "\n";
		}
	}
	std::cout << "\n\n";
}

void FPU::showProc() {
	std::cout << std::string(115, '-') + "\n\n";

	std::cout << "IR: " << IR << "\n\n";

	std::cout << "Stack:\n";
	gtAllVlFrStck();

	std::cout << "RAM:\n";
	mem.showMem();

	std::cout << "PS: " << PS << "\n";

	std::cout << "PC: " << PC << "\n";

	std::cout << "TC: " << TC << "\n";

	std::cout << "\n" + std::string(115, '-');

}