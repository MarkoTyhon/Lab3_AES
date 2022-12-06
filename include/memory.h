#pragma once

#include <string>
#include "IEEE754.h"
#include "properties.h"
#include <iostream>

struct Memory
{
	Memory();

	void addToMem(IEEE754 el);
	void addToMem(int indx, IEEE754 el);
	IEEE754 getFromMem(int indx);
	IEEE754 getFromMem();
	void showMem();

	IEEE754 memory[RAM];
	int LEI;
};