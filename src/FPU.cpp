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
	PS = 0;
	if (cmd[1] != "NONE") {
		IR += " " + cmd[1];
		PS = getValue(1).sign;
	}
	++PC;
	
	

	showProc();
}

void FPU::tact2() {
	TC = 2;
	doCommand();
	if (cmd[1] != "NONE") 
		PS = getValue(1).sign;
	showProc();
}


IEEE754 FPU::gtNVlFrStck(int indx) {
	std::stack<IEEE754> loc_stack;
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
		if (cmd[indx].find("[") == std::string::npos) {
			return mem.getFromMem();
		}
		else {
			return mem.getFromMem(std::stoi(cmd[indx].substr(cmd[indx].length() - 2, 1).c_str()));
		}
	}
	else {
		double dbl_value = std::stod(cmd[indx].c_str());
		return IEEE754(dbl_value);
	}
}

void FPU::deleteLastElem() {
	std::stack<IEEE754> loc_stack;
	while (reg_stack.size() < STACK_LEN) {
		loc_stack.push(reg_stack.top());
		reg_stack.pop();
	}
	for (int i = 0; i < loc_stack.size(); i++) {
		reg_stack.push(loc_stack.top());
		loc_stack.pop();
	}
}

void FPU::toMemory() {
	mem.addToMem(gtNVlFrStck(STACK_LEN-1));
	deleteLastElem();
}

void FPU::opMOV() {
	if (reg_stack.size() == 8) {
		toMemory();
	}
	reg_stack.push(getValue(1));
}

void FPU::opLOAD() {
	IEEE754 val = reg_stack.top();
	reg_stack.pop();

	if (cmd[1].find("[") == std::string::npos)
		mem.addToMem(val);
	else
		mem.addToMem(std::stoi(cmd[1].substr(cmd[1].length() - 2, 1).c_str()), val);
}


void FPU::opCOPY() {
	reg_stack.push(reg_stack.top());	
}

void FPU::opREVERSE() {
	IEEE754 buf = reg_stack.top();
	reg_stack.pop();
	IEEE754 buf2 = reg_stack.top();
	reg_stack.pop();
	reg_stack.push(buf2);
	reg_stack.push(buf);
	
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
	else if (cmd[0] == "cos") {
		opLN();
	}
	else if (cmd[0] == "tg") {
		opTG();
	}
	else if (cmd[0] == "ln") {
		opCOS();
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

void FPU::opCOS() {
	IEEE754 y = reg_stack.top();
	reg_stack.pop();
	if (y == ieee.getNaN())
		std::cout << "NaN VALUE!" << "\n";
	else
		reg_stack.push(y.cos());
}


void FPU::opTG() {
	IEEE754 y = reg_stack.top();
	reg_stack.pop();
	if (y == ieee.getNaN())
		std::cout << "NaN VALUE!" << "\n";
	else
		reg_stack.push(y.tg());
}

void FPU::opLN() {
	IEEE754 y = reg_stack.top();
	reg_stack.pop();
	if (y == ieee.getNaN())
		std::cout << "NaN VALUE!" << "\n";
	else
		reg_stack.push(y.log());
}


void FPU::opADD() {
	IEEE754 x = reg_stack.top();
	reg_stack.pop();
	IEEE754 y = reg_stack.top();
	reg_stack.pop();
	if ((x == ieee.getNaN()) || (y == ieee.getNaN()))
		std::cout << "NaN VALUE!" << "\n";
	else
		reg_stack.push(x + y);
}

void FPU::opSUBTRACT() {
	IEEE754 x = reg_stack.top();
	reg_stack.pop();
	IEEE754 y = reg_stack.top();
	reg_stack.pop();
	if ((x == ieee.getNaN()) || (y == ieee.getNaN()))
		std::cout << "NaN VALUE!" << "\n";
	else 
		reg_stack.push(x - y);
}

void FPU::opMULT() {
	IEEE754 x = reg_stack.top();
	reg_stack.pop();
	IEEE754 y = reg_stack.top();
	reg_stack.pop();
	if ((x == ieee.getNaN()) || (y == ieee.getNaN()))
		std::cout << "NaN VALUE!" << "\n";
	else 
		reg_stack.push(x * y);
}

void FPU::opDIVIS() {
	IEEE754 x = reg_stack.top();
	reg_stack.pop();
	IEEE754 y = reg_stack.top();
	reg_stack.pop();
	if ((x == ieee.getNaN()) || (y == ieee.getNaN()))
		std::cout << "NaN VALUE!" << "\n";
	else 
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
	std::stack<IEEE754> loc_stack;
	loc_stack = reg_stack;
	IEEE754 buf;
	int ccount = -1;
	while (!loc_stack.empty()) {
		buf = loc_stack.top();
		std::cout << "\t" << buf <<"\n";
		loc_stack.pop();
	}
	if (reg_stack.size() < STACK_LEN) {
		for (int i = 0; i < STACK_LEN - reg_stack.size(); i++) {
			buf = buf.getNaN();
			std::cout << "\t" << buf << "\n";
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