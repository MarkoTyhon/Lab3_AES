#pragma once
#include "properties.h"
#include <algorithm>

template <typename T>
class Stack
{
public:
	Stack();

	int size();
	void copy();
	void reverse();
	void push(T elem);
	T top();
	T pop();
	bool empty();
	bool full();

	T stack[STACK_LEN];
	bool free[STACK_LEN];
	int LEI;
	int empt;
	
	T outOfStack;
};

template <typename T>
Stack<T>::Stack() {
	LEI = 0;
	std::fill(free, free + STACK_LEN, 1);
	
}

template <typename T>
bool Stack<T>::empty() {
	return LEI==0;
}

template <typename T>
bool Stack<T>::full() {
	return LEI>= STACK_LEN;
}

template <typename T>
T Stack<T>::top() {
	if (empty()) {
		std::cout << "Error: Stack is empty!" << "\n";
		exit(EXIT_FAILURE);
	}
	return stack[0];
}

template <typename T>
T Stack<T>::pop() {
	if (empty()) {
		std::cout << "Error: Stack is empty!" << "\n";
		exit(EXIT_FAILURE);
	}
	T buf = top();
	for (int i = 1; i < LEI; i++) {
		stack[i - 1] = stack[i];
	}
	--LEI;
	free[LEI] = true;
	return buf;
}


template <typename T>
void Stack<T>::push(T elem) {
	if (LEI == STACK_LEN)
		outOfStack = stack[STACK_LEN-1];
	for (int i = LEI; i > 1; i--) {
		if(i != STACK_LEN)
			stack[i] = stack[i - 1];
	}
	if (LEI != STACK_LEN) {
		++LEI;
		free[LEI] = false;
	}
	stack[0] = elem;
}

template <typename T>
void Stack<T>::copy() {
	push(top());
}

template <typename T>
void Stack<T>::reverse() {
	T local = stack[0];
	stack[0] = stack[1];
	stack[1] = local;
}

template <typename T>
int Stack<T>::size() {
	return LEI;
}