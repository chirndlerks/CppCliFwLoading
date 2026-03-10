#include <iostream>
#include "dotnethosting.h"
#include "..\CppCliLib\Test.h"
#include <string>

int main() {
	std::ignore = InitDotnet::InitDotNet();
	std::cout << MyTest(42);
	std::string str;
	std::getline(std::cin, str);
}