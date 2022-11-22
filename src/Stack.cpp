#include "Stack.h"
#include <iostream>
#include "properties.h"

template <typename T>
Stack<T>::Stack() {
	empt = -1;
}

template <typename T>
bool Stack<T>::empty(){
	return empt == -1;
}

template <typename T>
bool Stack<T>::full() {
	return free[STACK_LEN-1];
}

template <typename T>
T Stack<T>::top() {
	if (this->empty) {
		std::cout << "Error: Stack is empty!" << "\n";
		exit(EXIT_FAILURE);
	}
	return stack[0];
}

template <typename T>
T Stack<T>::pop() {
	if (this->empty) {
		std::cout << "Error: Stack is empty!" << "\n";
		exit(EXIT_FAILURE);
	}
	for (int i = 1; i < LEI; i++) {
		stack[i - 1] = stack[i];
	}
	--LEI;
	free[LEI] = true;
	return stack[0];
}

template <typename T>
void Stack<T>::push(T elem) {
	if (this->full()) {
		sendToMemory();
	}
	
	for (int i = LEI; i > 1; i--) {
		stack[i] = stack[i - 1];
	}
	stack[0] = elem;
	
}

template <typename T>
void Stack<T>::copy() {
	this->push(this->top());
}

template <typename T>

void Stack<T>::reverse() {
	T local = stack[0];
	stack[0] = stack[1];
	stack[1] = local;
}
