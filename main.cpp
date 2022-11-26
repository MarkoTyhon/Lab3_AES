#include "fileParser.h"

#include <vector>
#include <iostream>
#include "IEEE754.h"

int main() {

	IEEE754 i = IEEE754(1.1);
	std::cout << i << "\n";
	
	i.number[10] = 0;
	std::cout << i.number[10];

	return 0;
}