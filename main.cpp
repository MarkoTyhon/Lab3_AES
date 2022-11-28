#include "fileParser.h"

#include <vector>
#include <iostream>
#include "IEEE754.h"

int main() {

	IEEE754 i = IEEE754(0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000001);
	std::cout << i;


	return 0;
}