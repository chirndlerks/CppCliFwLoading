#include <iostream>
#include "..\CppCliLib1\Test.h"
#include "..\CppCliLib2\Test.h"
#include <string>

int main() {

	// multiple ALC test
	int state = 42;
	SetGlobalState(state);
	int currentState = GetGlobalState();
	std::cout << state;
	std::cout << '\n';
	std::cout << GetGlobalState();

	std::string str;
	std::getline(std::cin, str);
}