#include "Test.h"

int MyTest(int test) {
	return CSLib::Class1::Test(test);
}

public ref class MyTestWrapper sealed {
public:
	static int WrapperTest(int x) {
		return MyTest(x);
	}
};