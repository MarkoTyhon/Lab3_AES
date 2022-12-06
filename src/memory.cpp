#include "memory.h"

#include <string>
#include <iostream>
#include "properties.h"
#include "hexTranslator.h"

Memory::Memory() {
	for (int i = 0; i < RAM; i++) {
		memory[i] = IEEE754(0);
		LEI = 0;
	}
}

void Memory::addToMem(IEEE754 el) {
	if (LEI == RAM - 1)
		LEI = 0;
	memory[LEI] = el;
	++LEI;
}

void Memory::addToMem(int indx, IEEE754 el) {
	memory[indx] = el;
}

IEEE754 Memory::getFromMem(int indx) {
	return memory[indx];
}

IEEE754 Memory::getFromMem() {
	if (LEI != 0)
		return memory[LEI - 1];
	else
		return memory[LEI];
};

void Memory::showMem() {
	HexTranslator hex;
	int counter = 0;
	for (int j = 0; j < RAM; j++) {
		hex.numToHex(memory[j]);
		for (int i = 0; i < HEX_LEN; i++) {
			std::cout << hex.str_hex_num.at(i);
			if (((i % 2) == 1) && (i != HEX_LEN - 1)) std::cout << "|";
		}
		if (j != RAM - 1)std::cout << "|";
	}
	std::cout << "\n";
	for (int i = 0; i < RAM * HEX_LEN / 2; i++) {
		if (counter < 10) std::cout << 0;
		std::cout << counter;
		if (i != RAM * HEX_LEN / 2 - 1) std::cout << "|";
		++counter;
	}
	std::cout << "\n\n";

}