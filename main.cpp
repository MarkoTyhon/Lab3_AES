#include "fileParser.h"

#include <vector>
#include <iostream>
#include "IEEE754.h"

int main() {
	double a = 0.00000000000000000000000000000000000000000000000000000000000000000000000000000002;
	double b = -0.000000000000000000000000000000000000000000000000000000000000000000000000000000001;
	IEEE754 i = IEEE754(a);
	IEEE754 j = IEEE754(b);
	IEEE754 res = IEEE754(a+b);
	std::cout << i << "\n" << j << "\n";
	
	std::cout << (i * j) << "\n";
	std::cout << res << "\n";
	
}