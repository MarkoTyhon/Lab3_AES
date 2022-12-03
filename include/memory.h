#pragma once

#include <string>
#include "IEEE754.h"

#include "properties.h"
struct Memory
{
	Memory();

	void addToMem(IEEE754 el);
	void addToMem(int indx, IEEE754 el);
	IEEE754 getFromMem(int indx);
	void showMem();

	IEEE754 memory[8];
	int LEI;
};