#include <iostream>
#include "..\CppCliLoader\Test.h"
#include <string>

int main() {

	// multiple ALC test
	int state = 42;
	SetLoaderGlobalState(state);
	int currentState = GetLoaderGlobalState();
	std::cout << state;
	std::cout << '\n';
	std::cout << currentState;

	std::string str;
	std::getline(std::cin, str);
}