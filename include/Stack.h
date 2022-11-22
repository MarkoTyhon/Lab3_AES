#pragma once
#include "properties.h"

template <typename T>
class Stack
{
public:
	Stack();

	void sendToMemory();
	void copy();
	void reverse();
	void push(T elem);
	T top();
	T pop();
	bool empty();
	bool full();


private:
	T stack[STACK_LEN];
	bool free[STACK_LEN];
	int LEI;
	int empt;
};

